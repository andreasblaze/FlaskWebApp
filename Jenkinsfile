pipeline {

    agent { dockerfile true }

    stages {

        stage('Git Checkout'){
            steps{
                git branch: 'main', credentialsId: 'ssh_flask-app', url: 'git@github.com:andreasblaze/FlaskWebApp.git'
            }
        }
        /*
        stage('Build Docker Image') {
            steps{
                    sh 'docker build -t flask-web-app .'
                }
            }
        

        stage('Run Docker Container') {
            steps{
                    sh 'docker run -it -p 80:80 flask-web-app'
                }
            }
        

        stage('Prune') {
            steps{
                    sh 'docker image prune -a'
                }
            }
        */

        /*
         stage('Push docker image to DockerHub') {
            steps{
                withDockerRegistry(credentialsId: 'dockerhub-cred-for-jenkins', url: 'https://index.docker.io/v1/') {
                    sh '''
                        docker push gansizz/jenkins-images:0.1
                    '''
                }
            }
        }
        */

    }
}