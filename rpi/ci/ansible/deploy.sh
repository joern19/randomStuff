#!/bin/bash

NAME=$1
PODMAN_COMMAND=$2

NEW_CREATE_COMMAND="podman run -d --name $NAME --replace $PODMAN_COMMAND"

if $(podman container exists $NAME); then
  OLD_CREATE_COMMAND=$(podman inspect $NAME | jq -r '.[0].Config.CreateCommand | join(" ")')
  if [[ "$OLD_CREATE_COMMAND" == "$NEW_CREATE_COMMAND" ]]; then
    echo "Nothing changed. (Container may not be running tough)"
  else
    echo "Replacing container!"
    echo OLD: $OLD_CREATE_COMMAND
    echo NEW: $NEW_CREATE_COMMAND
    /bin/sh -c "$NEW_CREATE_COMMAND"
  fi
else
  echo "Container missing, starting."
  /bin/sh -c "$NEW_CREATE_COMMAND"
fi

