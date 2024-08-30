## Partie 1 Créer une partie client serveur

### Partie 1 validée. Si le client peut envoyer ping et que le server repond pong

Créer deux éxecutables un client un serveur,
Capable de communiquer entre eux,
Familles de fonctions a utiliser --socket,bind,listen,accept,connect,write,read,send et recv.
Soit single thread avec select/poll ou modéles multi thread avec std::thread,

----------------------------------------------------------------------------------------------------------------------------------------------------

Utilisation de select pour le mutli clients
Utilisation de std::vector pour gérer les clients connectés
Client serveur fonctionnel
Code procédurale peut-être refactoring en POO

### Partie 1 Done

## Partie 2 Traitements des commandes

### Partie 2 validée. Si le client permet l'envoie d'un fichier via une commande au serveur et qu'il puisse le récupérer via une commande sur le serveur 

#### Commandes client :
    ./lpf ip:port -upload filename (upload un fichier)
    ./lpf ip:port -download filename (download un fichier)

    ip:port : IP et Port du serveur. Exemple : 127.13.37.1:20215
    Filename : Nom du fichier a uploader ou à télécharger
