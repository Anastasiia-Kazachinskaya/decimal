(Docker)
```shell
docker build -t my-app .
docker run -it --rm -v "$(pwd)":/work my-app
cd src
make all
```