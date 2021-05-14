pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
                sh 'echo "Building..."'
                sh 'chmod +x build.sh'
                sh 'build.sh'
            }
        }
    }
}