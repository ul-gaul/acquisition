# acquisition
code pour le système d'acquisition et le contrôle des relais d'alimentation

## branchement du board avec ST-Link
Pour flasher le code sur le board (target), il faut brancher 5 câbles du 
SWD du STM32DISCOVERY vers le SWD du target. Du côté du target, les 5 pins
à connecter sont les 5 pins à partir de P1. La pin nommée "5V" ne doit pas
être branchée. Du côté du STM32DISCOVERY, les 5 pins à connecter sont les 
5 à partir du côté excluant la pin nommée "CN2". Les fils doivent être 
dans le même ordre, c'est-à-dire que lors que les mêmes câbles sont 
connectés lorsqu'on aligne les pins non-connectés ("5V" et "CN2"). 
