# P1

## Sådan bruger du github:


### Første gang
1. Gå til den mappe hvor du gerne vil have filerne ligger med 'cd'
2. kør 'git clone https://github.com/pandajakob/P1.git'
    - giver en lokal 'klon' af filerne

### Inden du arbejder
1. ALTID 'git pull' som det første
    - henter alle ændrer lavet mens du var væk
2. kør 'git branch *ny-branch*'
    - Laver en ny branch hvor du arbejder på dine ændringer
    - ALDRIG lav ændringer i main branch
3. kør 'git checkout *ny-branch*'
    - skifter dig over i din nye branch

### Når du er færdig med at arbejde
1. kør 'git status'
    - giver overblik over ændringerne
2. kør 'git add .'
    - tilføjer alle ændringer til staging area
3. kør 'git commit -m "*beskriv ændringer*"'
    - committer ændringerne lokalt
4. kør 'git push'
    - Skubber ændringerne op på github

### inde på github.com
1. Lav pull request
    - når du har pushet nogle ændringer, skal du oprette en pull request på github, så vi kan se dem.
    - Det giver os andre lov til at review ændringer, inden de bliver indført i hele dokumentet

### regler på vores github repo:
1. man kan ikke pushe direkte til main branchen
2. man kan ikke selv godkende til pull request.
3. man kan ikke godkende pull request, hvis programmet ikke virker (github test).

