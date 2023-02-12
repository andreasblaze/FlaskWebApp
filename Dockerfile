FROM python:buster 
ENV PYTHONUNBUFFERED 1

LABEL VERSION="1.0.0"
LABEL MAINTAINER="Andrii Bondariev"
LABEL EMAIL="andreybond13@gmail.com"
LABEL NAME="Flask Web Application"

WORKDIR /FlaskWebApp

ADD main.py .
COPY requirements.txt .

RUN pip install -r requirements.txt

COPY ./app ./app

EXPOSE 80

ENTRYPOINT [ "python3", "main.py" ]
