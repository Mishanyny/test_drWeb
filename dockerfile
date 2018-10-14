FROM ubuntu:latest

MAINTAINER test

RUN apt-get update -y
RUN apt-get install -y python3-pip python3-dev build-essential
COPY . /app
WORKDIR /app
RUN pip3 install flask
EXPOSE 8080
ENTRYPOINT ["python3"]
CMD ["server.py"]

