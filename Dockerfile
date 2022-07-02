FROM amazonlinux

RUN yum -y update
RUN yum -y install git
RUN yum -y install httpd

CMD ["/usr/sbin/httpd","-D","FOREGROUND"]


FROM python:3.9.7-alpine3.14

LABEL VERSION="1.0.0"
LABEL MAINTAINER="Andrey Bondariev"
LABEL EMAIL="andreybond13@gmail.com"
LABEL NAME="Flask Web Application"

RUN git clone https://github.com/andreasblaze/FlaskWebApp.git > /var/www/

WORKDIR /
RUN pip install -r requirements.txt

COPY . /var/www/FlaskWebApp

EXPOSE 80
ENTRYPOINT [ "python3" ]
CMD [ "main.py" ]