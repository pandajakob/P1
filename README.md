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

### Inde på github.com
1. Lav pull request
    - når du har pushet nogle ændringer, skal du oprette en pull request på github, så vi kan se dem.
    - Det giver os andre lov til at review ændringer, inden de bliver indført i hele dokumentet

### Regler på vores github repo:
1. man kan ikke pushe direkte til main branchen
2. man kan ikke selv godkende til pull request.
3. man kan ikke godkende pull request, hvis programmet ikke virker (github test).

### Brugbare Git-kommandoer
- `git status`: Viser status for ændringer i mappen. Brug den til at få et overblik over, hvad der er ændret, og hvad der skal tilføjes eller committes.
- `git add .`: Tilføjer alle ændringer til staging area, så de kan committes.
- `git commit -m "*beskriv ændringer*"`: Gemmer ændringerne lokalt med en besked, der beskriver, hvad der er lavet.
- `git push`: Skubber ændringerne fra din lokale branch op til GitHub.
- `git pull`: Henter og opdaterer din lokale kopi af projektet med ændringer fra GitHub.
- `git log`: Viser commit-historikken. Bruges til at se, hvilke ændringer der er blevet lavet tidligere, og af hvem.
- `git diff`: Viser ændringer mellem din lokale kode og den sidste commit. Kan også bruges til at se ændringer i et bestemt filområde, som f.eks. `git diff *filnavn*`.
- `git branch`: Viser en liste over alle branches i projektet. Den aktive branch vises med en stjerne (*) ved siden af navnet.
- `git branch -d *branchnavn*`: Sletter en lokal branch, når du er færdig med den. Sørg for, at alle ændringer er pushet, inden du sletter en branch.
- `git stash`: Gemmer dine ændringer midlertidigt, så du kan vende tilbage til dem senere. Brug `git stash apply` for at få dem tilbage igen.
- `git rebase *branchnavn*`: Gør det muligt at flytte commits fra én branch til en anden. Bruges ofte til at holde dine commits opdateret med main-branchen.
- `git merge *branchnavn*`: Slår ændringer fra en anden branch sammen med den nuværende branch.
- `git reset --hard`: Annullerer alle ændringer, som ikke er committed endnu (pas på med denne, da ændringerne ikke kan genskabes).
- `git rm --cached *filnavn*`: Fjerner filer fra Git uden at slette dem fra mappen.