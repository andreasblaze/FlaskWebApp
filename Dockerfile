FROM python:3.9.7-alpine3.14
ENV PYTHONUNBUFFERED 1

LABEL VERSION="1.0.0"
LABEL MAINTAINER="Andrey Bondariev"
LABEL EMAIL="andreybond13@gmail.com"
LABEL NAME="Flask Web Application"

WORKDIR /FlaskWebApp

ADD main.py .
COPY requirements.txt .

RUN pip install -r requirements.txt

COPY ./app ./app

EXPOSE 80

CMD [ "python3", "./main.py" ]
