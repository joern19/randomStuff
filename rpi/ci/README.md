# Install
- Startup the docker-compose.yml with podman in the pi user (rootless).
- ./render.sh
- fly -t kaboom set-pipeline -v SSHKEY=$B64_ENCODED_CONCOURSE_SSH_KEY -p main -c output/concourse-pipeline/templates/main.yaml

# Secrets
Right now I think I will only need the ssh key of the pi.
Without a proper var_source, it will be stored unencrypted in the pipeline when deploying.
I guess that's fine for the use case I have. Only reachable locally anyway.
