//
// Created by Richard Harman on 1. 12. 2022.
//

#include "main.h"

class EnergyToBattery : public Process{
    void Behavior(){
        double p = Uniform(0, 100);
        if(p <= up){
            while(powerGenerated != 0 && !Battery.Empty()){
                Leave(Battery, 1);
                powerGenerated -= 1.0;
            }
        } else {
            return;
        }// else it's leaving the system
    }
};

class DayLight : public Event{
    void Behavior(){
        double p = Uniform(0, 100);
        if(p > 36){
            //printf("Cloudy weather...\n");
            powerGenerated += std::round(((((std::round(pw-0.5)*1000000*60)/365)/10)/60)*0.825);
            //printf("%f\n", powerGenerated);
        } else {
            //printf("Sunny weather...\n");
            powerGenerated += std::round((((std::round(pw-0.5)*1000000*60)/365)/10)/60);
            //printf("%f\n", powerGenerated);
        }
        (new EnergyToBattery)->Activate();
    }
};

class SystemFunctions : public Process{
    void Behavior(){
        if(times >= 1){
            times -= 1;
            double p = Uniform(0, 100);
            if(p > 49.3){
                (new DayLight)->Activate();
            } else {
                return;
            }// else it's leaving the system
        }
    }
};

class Timing : public Process{
    void Behavior(){
        Seize(Time_f);
        Wait(generatePower);
        Release(Time_f);
    }
};

class Generator_time : public Event{
    void Behavior(){
        times += 1;
        (new Timing)->Activate();
        (new SystemFunctions)->Activate();
        Activate(simlib3::Time + generatePower);
    }
};

class FailureOccured : public Process{
    void Behavior(){
        printf("Nastala porucha...\n");
        Priority = 1;
        Seize(Time_f);
        Seize(Usage);
        Seize(Failure);
        Wait(Exponential(6 HOUR));
        Release(Failure);
        Release(Usage);
        Release(Time_f);
        systemAge = 0;
    }
};

class FailurePicker : public Process{
    void Behavior(){
        if(failure >= 1){
            failure -= 1;
            double p = Uniform(0, 100);
            if(p <= (fs+(systemAge*1.1))){
                (new FailureOccured)->Activate();
            } else {
                return;
            }// else it's leaving the system
        }
    }
};

class Fail : public Process{
    void Behavior(){
        Seize(Failure);
        Wait(failureOccurance);
        Release(Failure);
        systemAge += 1;
    }
};

class Generate_Failure : public Event{
    void Behavior(){
        failure += 1;
        (new Fail)->Activate();
        (new FailurePicker)->Activate();
        Activate(simlib3::Time + failureOccurance);
    }
};

class RevisionOccured : public Process{
    void Behavior(){
        printf("Potrebné revízia...\n");
        Priority = 2;
        Seize(Revision);
        Seize(Time_f);
        Seize(Usage);
        Wait(revisionLength);
        Release(Usage);
        Release(Time_f);
        Release(Revision);
    }
};

class RevisionNeeded : public Process{
    void Behavior(){
        if(revision >= 1){
            revision -= 1;
            (new RevisionOccured)->Activate();
        }
    }
};

class RevisionWait : public Process{
    void Behavior(){
        Seize(Revision);
        Wait(revisionOccurance);
        Release(Revision);
        revision += 1;
    }
};

class Generate_Revision : public Event{
    void Behavior(){
        (new RevisionWait)->Activate();
        (new RevisionNeeded)->Activate();
        Activate(simlib3::Time + revisionOccurance);
    }
};

class UseElectricity : public Process{
    void Behavior(){
        while(powerUsed > 10 && !Battery.Full()){
            Enter(Battery, 10);
            powerUsed -= 10;
        }
        Enter(Battery, (unsigned long)powerUsed);
        powerUsed -= powerUsed;
    }
};

class UsageWait : public Process{
    void Behavior(){
        Seize(Usage);
        Wait(generateUsage);
        Release(Usage);
        powerUsed += round((((hw*1000*60)/365)/24)/60);
    }
};

class Generate_Usage : public Event{
    void Behavior(){
        (new UsageWait)->Activate();
        (new UseElectricity)->Activate();
        Activate(simlib3::Time + generateUsage);
    }
};

class UseNetworkElectricity : public Process{
    void Behavior(){
        if(powerUsed > 299 && Battery.Empty()){
            powerUsed -= 300;
        } else if(powerUsed < 299 && Battery.Empty()){
            powerUsed -= powerUsed;
        }
    }
};

class NetworkElectricity : public Process{
    void Behavior(){
        Seize(Network);
        Release(Network);
    }
};

class Generate_Network_Electricity : public Event{
    void Behavior(){
        (new NetworkElectricity)->Activate();
        (new UseNetworkElectricity)->Activate();
        Activate(simlib3::Time + 1);
    }
};

class EmptyBattery : public Process{
    void Behavior(){
        Enter(Battery, (unsigned long)bc*1000*60);
    }
};

int argParse(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    if(argParse(argc, argv) == -1){
        return -1;
    }
    printf("Starting simulation...\n");
    SetOutput(output.c_str());
    printf("Output set...\n");
    Init(0, (double)simulation_time);
    printf("Simulation initialised...\n");
    (new EmptyBattery)->Activate();
    (new Generator_time)->Activate();
    (new Generate_Failure)->Activate();
    (new Generate_Revision)->Activate();
    (new Generate_Usage)->Activate();
    (new Generate_Network_Electricity)->Activate();

    Run();

    Time_f.Output();
    Revision.Output();
    Failure.Output();
    Usage.Output();
    Network.Output();
    Battery.Output();

    printf("powerGenerated: %f\n", powerGenerated);

    return 0;
}

int argParse(int argc, char *argv[]){
    char* ptr;
    long long tmpLong = 0;
    int opt = 0;

    if(argc == 2 && (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0)){
        printf("Použitie: [pw] [up] [pp] [bc] [hw] [kwp] [fs] [fp] [rp] [of]\n"
               "[pw] Sila solárneho panelu (kWp)\n"
               "[up] Percento využitia energie (Percent) \n"
               "[pp] Cena panelu (Kč)\n"
               "[bc] Kapacita batérie (kWh)\n"
               "[hw] Spotreba domu (kWh)\n"
               "[kwp] Cena za kilowatt (Kč)\n"
               "[fs] Pravdepodobnost chyby systému (Percent)\n"
               "[fp] Cena opravy chyby (Kč)\n"
               "[rp] Cena revízie (Kč)\n"
               "[of] Názov súboru pre zápis výsledkov\n"
               "- Všetky parametre sa zadávajú vo formáte FLOAT.\n");
        return 0;
    } else if(argc != 11) {
        printf("Použitie: [pw] [up] [pp] [bc] [hw] [kwp] [fs] [fp] [rp] [of]\n"
               "[pw] Sila solárneho panelu (kWp)\n"
               "[up] Percento využitia energie (Percent) \n"
               "[pp] Cena panelu (Kč)\n"
               "[bc] Kapacita batérie (kWh)\n"
               "[hw] Spotreba domu (kWh)\n"
               "[kwp] Cena za kilowatt (Kč)\n"
               "[fs] Pravdepodobnost chyby systému (Percent)\n"
               "[fp] Cena opravy chyby (Kč)\n"
               "[rp] Cena revízie (Kč)\n"
               "[of] Názov súboru pre zápis výsledkov\n"
               "- Všetky parametre sa zadávajú vo formáte FLOAT.\n");
        fprintf(stderr, "\nNeznámy parameter\n");
        return -1;
    } else {
        pw = std::stod(argv[1]);
        up = std::stod(argv[2]);
        pp = std::stod(argv[3]);
        bc = std::stod(argv[4]);
        hw = std::stod(argv[5]);
        kwp = std::stod(argv[6]);
        fs = std::stod(argv[7]);
        fp = std::stod(argv[8]);
        rp = std::stod(argv[9]);
        output = argv[10];
    }

    Battery.SetCapacity((unsigned long)bc*1000*60);

    return 0;
}

