# Utilise une image de base contenant GCC
FROM gcc:latest

# Crée un répertoire de travail dans le conteneur
WORKDIR /usr/src/myapp

# Copie tout le contenu de ton répertoire local dans le conteneur
COPY main.c .

# Compile le programme C lorsque tu démarres le conteneur
RUN gcc -o http_server main.c

EXPOSE 8080
# Commande par défaut qui exécute ton programme compilé
CMD ["./http_server"]
