sed -ie '/creds/d' $HOME/.docker/config.json 
rm -rf ~/Library/Containers/com.docker.docker
mkdir -p ~/goinfre/Docker/Data
ln -s ~/goinfre/Docker ~/Library/Containers/com.docker.docker
docker build -t "container" .
docker run -it -v $PWD:/tmp -w /tmp container