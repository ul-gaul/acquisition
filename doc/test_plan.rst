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
================= =================================== ==========================
Fonction Testée   Méthode de test                     Sortie prévue
================= =================================== ==========================
rfd900_write      À l'aide d'un module de réception,  Tous les RocketPackets
                  configuré avec les bons paramètres  sont reçus sans erreurs
                  UART, afficher les données reçues   de transmission.
                  par le RFD900. Avec le système
                  d'acquisition, envoyer un
                  RocketPacket vide (ou contenant des
                  données arbitraires) à une
                  fréquence de 10 Hz.
================= =================================== ==========================

    Tests en intégration


Acquisition des données du moteur
---------------------------------

    Description


    Tests de développement


    Tests en intégration


Acquisition des données du IMU 10DOF
------------------------------------

    Description


    Tests de développement


    Tests en intégration


Sauvegarde des données
----------------------

    Description


    Tests de développement


    Tests en intégration
