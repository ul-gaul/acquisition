=====================================================
Plan de test pour le système d'acquisition de données
=====================================================

Définition
----------

Ce plan de test vise à simplifier le suivi du progrès sur le système
d'acquisition de données de la fusée. Il consiste en une courte description de
chacune des fonctionnalités qui composent le système, suivi d'une liste
détaillée des tests qui devront passer pour s'assurer de la *sanity* du
sous-système.

LEDs de debug
-------------

    Description
Les LEDs de debug sont les 6 LEDs de couleur sur le circuit d'acquisition qui
servent à vérifier le bon fonctionnement du système. Quand les LEDs clignotent,
on est assurés que le code est exécuté.

    Tests de développement
Dans le *main*, initialiser les LEDs à l'aide de la fonction *init_leds* et
ajouter des calls aux fonctions *set_led_on* et *set_led_off*. Ensuite, utiliser
le mode debug de l'IDE utilisé pour s'assurer que **toutes** les LEDs ont le
comportement prévu, selon le tableau suivant:

==================== ===========================
Fonction appelée     Comportement prévu
==================== ===========================
*set_led_on(LEDX)*   La LED numéro X est allumée
*set_led_off(LEDX)*  La LED numéro X est éteinte
==================== ===========================

    Tests en intégration
.. TODO

Communication RF
----------------

    Description
Le communication RF (*Radio Frequency*) est utilisée par le système pour
effectuer le transfert des données de vol vers la station au sol (SAS). La
couche liaison de cette communication est assurée par le module RFD900. Comme
son nom l'indique, ce module transmet dans la bande de fréquence des 900 MHz.
Pour la transmission, le module prend en entrée un signal UART, dont la
configuration doit être la même que celle du RFD900. Le même principe s'applique
pour la réception des données.

À la couche transport, la communication utilise une structure de données, nommée
*RocketPacket*, qui est sérialisée avant d'être transmise par UART. La
structure est ensuite reformée lors de la réception.

    Tests de développement
================== =================================== ==========================
Fonction Testée    Méthode de test                     Sortie prévue
================== =================================== ==========================
rfd900_write       À l'aide d'un module de réception,  Tous les RocketPackets
                   configuré avec les bons paramètres  sont reçus sans erreurs
                   UART, afficher les données reçues   de transmission.
                   par le RFD900. Avec le système
                   d'acquisition, envoyer un
                   RocketPacket vide (ou contenant des
                   données arbitraires) à une
                   fréquence de 10 Hz.
*send_packet*
*serialize_packet*
================== =================================== ==========================

    Tests en intégration


Acquisition des données du moteur
---------------------------------

    Description
Afin de s'assurer que le lancement de la fusée est sécuritaire, il faut obtenir 
en temps réel certaines données concernant le système de moteur hybride. Pour
être exact, la pression de la valve de la bonbonne de NOS est la donnée 
essentielle pour atteindre ce but. Ensuite, la température ambiante autour de 
la bonbonne est également importante, mais n'est pas essentielle puisqu'elle
peut être calculée selon la pression.

Pour obtenir ces données, des capteurs, placés dans le système de moteur, seront
lus régulièrement par le système d'acquisition et leurs données seront 
communiquées à la SAS.

    Tests de développement
.. TODO ajouter test isolé pour capteur de température
============================== ============================== ==============================
Test                           Méthode                        Interprétation des résultats
============================== ============================== ==============================
Lire capteur pression          En mode debug, s'assurer que   Si la pression lue semble être
                               la fonction de lecture de      bonne, valider avec l'équipe
                               pression retourne une valeur   propulsion que les données 
                               entre 0 et 10 MPa              sont utilisables.
============================== ============================== ==============================

    Tests en intégration
.. TODO ajouter tests pour: intégration dans le rocket packet, transmission au sol, 
..      interprétation par le soft de la SAS, lecture avec les vrais capteurs dans 
..      dans le vrai moteur (au repo)


Acquisition des données de bord
------------------------------------

    Description
Les données de bord sont les données non-essentielles au vol de la fusée, mais 
qui sont néamoins intéressantes à avoir pour le post-traitement du vol. Ces 
données comprennent:
  - Accélération sur 3 axes
  - Vitesse angulaire sur 3 axes
  - Champs magnétiques sur 3 axes
  - positionnement GPS
  - température

Ces données doivent idéalement être échantillonnées et transmises à au moins 10 Hz.
Certains capteurs étant plus rapides que d'autres (e.g. le 10DOF est environ 10 fois
plus rapide que le GPS), il est possible d'envoyer les packets à une fréquence proche
de celle du capteur le plus rapide, en mettant à jour les autres données lorsqu'elles
sont prêtes.

    Tests de développement
.. TODO: tableau indiquant de tester chaque driver de chaque capteur en mode debug 
..       (voir section sur données du moteur)

    Tests en intégration
.. TODO: ajouter tests: intégration de chaque donnée dans le rocketpacket, tester 
..       transmission au sol, tester avec SAS, tester transmission à l'intérieur 
..       d'un fuselage quelconque

Sauvegarde des données
----------------------

    Description
.. TODO fuck this

    Tests de développement


    Tests en intégration
