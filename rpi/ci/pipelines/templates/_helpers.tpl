{{- define "pipelines.task" -}}
config:
  platform: linux
  image_resource:
    type: registry-image
    source:
      repository: ghcr.io/joern19/concourse-build
      tag: 4
  run:
    path: sh
    args: [ "-c", {{ . | quote }} ]
{{- end }}
{{- define "pipelines.remoteTask" -}}
config:
  platform: linux
  image_resource:
    type: registry-image
    source:
      repository: ghcr.io/joern19/concourse-build
      tag: 4
  run:
    path: sh
    args:
    - "-c"
    - |
      mkdir -p $HOME/.ssh &&
      echo ((SSHKEY)) | base64 -d > $HOME/.ssh/id_rsa &&
      chmod 600 $HOME/.ssh/id_rsa
      {{ printf "ssh -4 -o BatchMode=yes -o StrictHostKeyChecking=no pi@kaboom.l.joern19.de \"%s\"" . }}
{{- end }}
{{- define "pipelines.commands" -}}
{{- (join ";" .) -}}
{{- end }}
