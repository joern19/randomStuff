#!/bin/sh
# mkdir -p output

# Only allow .yaml, not .yml
pipelines=$(ls templates/pipelines/*.yaml)

pipelineNames=""
for pipeline in $pipelines; do
  filename=$(basename $pipeline)
  pipelineName=${filename::-5}
  pipelineNames="$pipelineNames$pipelineName,"
done
pipelineNames="${pipelineNames::-1}"
echo "Pipelines: $pipelineNames"

helm template --output-dir output --set "pipelines={$pipelineNames}" --debug .
