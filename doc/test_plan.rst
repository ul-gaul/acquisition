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


    Tests de développement


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
