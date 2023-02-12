def img
pipeline {
environment {
        registry = "blazeid/flask-web-app"
        registryCredential = 'docker-hub'
        dockerImage = ''
    }
    agent any

    stages {

        stage('Git Checkout'){
            steps{
                git branch: 'main', credentialsId: 'ssh_flask-app', url: 'git@github.com:andreasblaze/FlaskWebApp.git'
            }
        }
        
        stage('Build Docker Image') {
            steps{
                    script {
                    img = registry + ":${env.BUILD_ID}"
                    println ("${img}")
                    dockerImage = docker.build("${img}")
                    }
                }
            }
        
        stage('Push To DockerHub') {
            steps {
                script {
                    docker.withRegistry( 'https://registry.hub.docker.com ', registryCredential ) {
                        dockerImage.push()
                    }
                }
            }
        }

        stage('Deploy') {
            steps {
                script {
                    def stopcontainer = "docker stop ${JOB_NAME}"
                    def delcontName = "docker rm ${JOB_NAME}"
                    def delimages = 'docker image prune -a --force'
                    def drun = "docker run -d --name ${JOB_NAME} -p 80:80 ${img}"
                    println "${drun}"
                    sshagent(['ssh_flask-app']) {
                        sh returnStatus: true, script: "ssh -o StrictHostKeyChecking=no docker@192.168.1.16 ${stopcontainer} "
                        sh returnStatus: true, script: "ssh -o StrictHostKeyChecking=no docker@192.168.1.16 ${delcontName}"
                        sh returnStatus: true, script: "ssh -o StrictHostKeyChecking=no docker@192.168.1.16 ${delimages}"

                    // some block
                        sh "ssh -o StrictHostKeyChecking=no blaze@127.0.0.1 -p 2223 ${drun}"
                    }
                }
            }
        }

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