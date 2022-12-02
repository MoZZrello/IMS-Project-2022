//
// Created by Richard Harman on 1. 12. 2022.
//

#include "main.h"

int argParse(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    if(argParse(argc, argv) == -1){
        return -1;
    }

    return 0;
}

int argParse(int argc, char *argv[]){
    char* ptr;
    long long tmpLong = 0;
    int opt = 0;

    if(argc == 2 && (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0)){
        printf("Použitie: [pw] [up] [pp] [bc] [hw] [kwp] [fs] [fp] [rr] [rl] [rp] [of]\n"
               "[pw] Sila solárneho panelu (kWp)\n"
               "[up] Percento využitia energie (Percent) \n"
               "[pp] Cena panelu (Kč)\n"
               "[bc] Kapacita batérie (kWh)\n"
               "[hw] Spotreba domu (kWh)\n"
               "[kwp] Cena za kilowatt (Kč)\n"
               "[fs] Pravdepodobnost chyby systému (Percent)\n"
               "[fp] Cena opravy chyby (Kč)\n"
               "[rr] Revízia kazdych X rokov (rok)\n"
               "[rl] Dĺžka revízie (min)\n"
               "[rp] Cena revízie (Kč)\n"
               "[of] Názov súboru pre zápis výsledkov\n"
               "- Všetky parametre sa zadávajú vo formáte FLOAT.\n");
        return 0;
    } else if(argc != 13) {
        printf("Použitie: [pw] [up] [pp] [bc] [hw] [kwp] [fs] [fp] [rr] [rl] [rp] [of]\n"
               "[pw] Sila solárneho panelu (kWp)\n"
               "[up] Percento využitia energie (Percent) \n"
               "[pp] Cena panelu (Kč)\n"
               "[bc] Kapacita batérie (kWh)\n"
               "[hw] Spotreba domu (kWh)\n"
               "[kwp] Cena za kilowatt (Kč)\n"
               "[fs] Pravdepodobnost chyby systému (Percent)\n"
               "[fp] Cena opravy chyby (Kč)\n"
               "[rr] Revízia kazdych X rokov (rok)\n"
               "[rl] Dĺžka revízie (min)\n"
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
        rr = std::stod(argv[9]);
        rl = std::stod(argv[10]);
        rp = std::stod(argv[11]);
        output = argv[12];
    }

    return 0;
}
