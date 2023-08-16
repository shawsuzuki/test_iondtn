docker rm -f $(docker ps -q -a)
docker rmi -f $(docker images -q)

docker-compose  build --no-cache

docker run -it -d shaw_wip-dtn-server-1
docker run -it -d shaw_wip-dtn-server-2