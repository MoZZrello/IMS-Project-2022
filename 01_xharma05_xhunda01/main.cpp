//
// Created by Richard Harman on 1. 12. 2022.
//

#include "main.h"

class EnergyToBattery : public Process{
    void Behavior(){
        inBattery += generated;
    }
};

class DayLight : public Event{
    void Behavior(){
        double p = Uniform(0, 100);
        if(p > 36){
            //printf("Cloudy weather...\n");
            generated = std::round(((((std::round(pw-0.5)*1000000*60)/365)/10)/60)*0.825);
            powerGenerated += generated;
            //printf("%f\n", powerGenerated);
        } else {
            //printf("Sunny weather...\n");
            generated = std::round((((std::round(pw-0.5)*1000000*60)/365)/10)/60);
            powerGenerated += generated;
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
        //printf("Nastala porucha...\n");
        Priority = 1;
        Seize(Time_f);
        Seize(Usage);
        Seize(Failure);
        Wait(Exponential(6 HOUR));
        Release(Failure);
        Release(Usage);
        Release(Time_f);
        systemAge = 0;
        pp += fp;
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
        years += 1;
        if(fromBattery/60000/years <= 4000){
            pp += 588;
        } else {
            pp += 5988;
        }
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

// kontrola revizora wait 30 minut a potom procesy vrati
class RevisionOccured : public Process{
    void Behavior(){
        //printf("Potrebná revízia...\n");
        Priority = 2;
        Seize(Revision);
        Seize(Time_f);
        Seize(Usage);
        Wait(revisionLength);
        Release(Usage);
        Release(Time_f);
        Release(Revision);
        pp += rp;
    }
};

//po 4 rokoch sa ide vykonatt revizia
class RevisionNeeded : public Process{
    void Behavior(){
        if(revision >= 1){
            revision -= 1;
            (new RevisionOccured)->Activate();
        }
    }
};

//cakam 4 roky na reviziu
class RevisionWait : public Process{
    void Behavior(){
        Seize(Revision);
        Wait(revisionOccurance);
        Release(Revision);
        revision += 1;
    }
};

//vygenerujem reviziu
class Generate_Revision : public Event{
    void Behavior(){
        (new RevisionWait)->Activate();
        (new RevisionNeeded)->Activate();
        Activate(simlib3::Time + revisionOccurance);
    }
};

class UseElectricity : public Process{
    void Behavior(){
        //printf("Míňame energiu... %f\n", powerUsed);
        if(inBattery >= powerUsed){
            fromBattery += powerUsed;
            inBattery -= powerUsed;
            powerUsed -= powerUsed;
        } else {
            fromNetwork += powerUsed;
            powerUsed -= powerUsed;
        }
        while((int)powerUsed != 0){
            if(inBattery > 0){
                powerUsed -= 1;
                inBattery -= 1;
                fromBattery += 1;
            } else {
                powerUsed -= 1;
                fromNetwork += 1;
            }
        }
    }
};

//spotreba ->opyta sa na 1 a vrati naspat 1
class UsageWait : public Process{
    void Behavior(){
        Seize(Usage);
        Wait(generateUsage);
        Release(Usage);
        powerUsed += ((((hw*1000*60)/365)/24)/60)/100*(100-es);
    }
};

class Generate_Usage : public Event{
    void Behavior(){
        (new UsageWait)->Activate();
        (new UseElectricity)->Activate();
        Activate(simlib3::Time + generateUsage);
    }
};


int argParse(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    if(argParse(argc, argv) == -1){
        return -1;
    }
    printf("Začínam simuláciu...\n");
    SetOutput(output.c_str());
    printf("Nastavený výstupný soubor...\n");
    Init(0, (double)simulation_time);
    printf("Simulácia inicializovaná...\n");
    (new Generator_time)->Activate();
    (new Generate_Failure)->Activate();
    (new Generate_Revision)->Activate();
    (new Generate_Usage)->Activate();

    Run();

    Time_f.Output();
    Revision.Output();
    Failure.Output();
    Usage.Output();

    printf("Koniec simulácie...\n");
    printf("\nPanely vytvorili: %f kWh/ročne\n", powerGenerated/60000/years);
    printf("Z virtuálnej batérie sa spotrebovalo: %f kWh/ročne\n", fromBattery/60000/years);
    printf("Zo siete sa spotrebovalo: %f kWh/ročne\n", fromNetwork/60000/years);
    printf("V batérii sa nachádza na konci: %f kWh\n", inBattery/60000);
    printf("Šetrí ročne: %f Kč\n",(fromBattery/60000/25)*kwp);
    double returnOfPanelPrice = pp/((fromBattery/60000/25)*kwp);
    printf("Návratnosť: %f rokov\n\n", returnOfPanelPrice);

    return 0;
}

int argParse(int argc, char *argv[]){
    char* ptr;
    long long tmpLong = 0;
    int opt = 0;

    if(argc == 2 && (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0)){
        printf("Použitie: [pw] [pp] [hw] [es] [kwp] [fs] [fp] [rp] [of]\n"
               "[pw] Sila solárneho panelu (kWp)\n"
               "[pp] Cena panelu (Kč)\n"
               "[hw] Spotreba domu (kWh)\n"
               "[es] Šetrenie spotreby (Percent)\n"
               "[kwp] Cena za kilowatt (Kč)\n"
               "[fs] Pravdepodobnost chyby systému (Percent)\n"
               "[fp] Cena opravy chyby (Kč)\n"
               "[rp] Cena revízie (Kč)\n"
               "[of] Názov súboru pre zápis výsledkov\n"
               "- Všetky parametre sa zadávajú vo formáte FLOAT.\n");
        return 0;
    } else if(argc != 10) {
        printf("Použitie: [pw] [pp] [hw] [es] [kwp] [fs] [fp] [rp] [of]\n"
               "[pw] Sila solárneho panelu (kWp)\n"
               "[pp] Cena panelu (Kč)\n"
               "[hw] Spotreba domu (kWh)\n"
               "[es] Šetrenie spotreby (Percent)\n"
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
        pp = std::stod(argv[2]);
        hw = std::stod(argv[3]);
        es = std::stod(argv[4]);
        kwp = std::stod(argv[5]);
        fs = std::stod(argv[6]);
        fp = std::stod(argv[7]);
        rp = std::stod(argv[8]);
        output = argv[9];
    }

    //Battery.SetCapacity((unsigned long)bc*1000*60);

    return 0;
}

