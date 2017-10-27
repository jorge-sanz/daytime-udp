# Daytime UDP
DAYTIME protocol iterative server and client using UDP sockets written in C. Project developed for learning purposes in Networks and Services Architecture subject of Software Engineering degree at University of Valladolid.

## Requirements
- [Docker](https://www.docker.com/) on your machine

## Development
### Getting started
First, set up the environment.
```bash
git clone https://gitlab.com/jorgesanz/daytime-udp
cd daytime-udp
docker run --name=ubuntu-daytime-udp -it -v ~/Documents/Workspace/daytime-udp:/home ubuntu
apt-get update
apt-get install build-essential
```

For the next times, how to deploy local environment?
```bash
docker start ubuntu-daytime-udp
docker exec -it ubuntu-ars bash
``` 
