# MCD Au format MeriseAcide.

# Déclaration d'une entité.
[Ville]

# Un + déclare CodePostal comme identifiant (clé).
+CodePostal

# Déclaration d'un champ.
Nom

# Une autre entité.
[Personne]

+ID_ # Le tiret bas indique que la clé
     # devra être suffixée du nom
     # de l'entité. Dans le cas
     # présent le résultat sera : ID_Personne.

Nom_ # Le principe du suffixe automatique
     # s'applique aussi au champs.

     # Il est possible de préfixer en
     # plaçant le tiret bas devant.
_Prénom

     # Ici, le type est explicitement défini,
     # directement en SQL.
NumeroRue    INTEGER(3)
     # Lorsque le type n'est pas défini,
     # Un type adaptée aux clés ou
     # au champs sera choisi en fonction
     # du type de base de données
     # choisis pour la sortie SQL.

# D'autres champ...
NomRue        VARCHAR(70)
DateNaissance DATE

# Voici maintenant une association.
# S'écrit comme se présente sur un MCD.
Personne    0,2    Hériter    0,n    Personne
Personne    1,1    Habiter    0,n    Rue
#   ^       ^         ^        ^      ^
# Entité   /  Nom d'association \    Entité
#    Cardinalitée         Cardinalitée

[Quartier]
+ID_          INTEGER(5) NOT NULL
Nom

# Ici, une association relative est définie.
Ville        0,n    Posséder    (1,1)    Quartier

[Rue]
+ID_
Nom

Quartier    0,n    Avoir        (1,1)    Rue

[Abonnement]
+ID_
Prix           NUMERIC(5,2)
DateAbonnement DATE

Abonnement    1,1    Payer       0,n    Personne

# Ici, une association ternaire est définie.
Abonnement    0,n    Concerne    0,n    Personne, Matière
                     DateInscription    DATE
# Un champ d'association aussi défini, les règles sont
# identique à celles des entités. 

# Tests du parser :
# ⇓⇓⇓⇓⇓⇓⇓⇓⇓⇓⇓⇓⇓⇓⇓⇓⇓

 [ Matière ]
+          ID_
  Nom

Abonnement    1,1    Associer    0,n    Matière

[Niveau   ]  
  +ID_
  Nom

         Abonnement           1,1    Commencer    0,n    Niveau     
Abonnement     1,1      Terminer    0,n    Niveau

  [     Versement]
   +         ID_          
      Somme        NUMERIC(5,2) 

Versement            1,1 Verser 0,n Abonnement   

[ModePayement]
  
    +     _ID      
       
       Nom      

ModePayement    0,n     Utiliser 1,1 Versement

