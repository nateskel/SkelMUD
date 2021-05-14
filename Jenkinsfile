pipeline {
    agent any
    stages {
        stage('Build') {
            steps {
                sh 'echo "Building..."'
                sh 'cd /SkelMUD'
                sh 'make'
            }
        }
    }
}