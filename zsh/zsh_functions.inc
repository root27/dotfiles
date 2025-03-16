setopt aliases



mcd() {
  mkdir -p "${1:?Need to specify an argument}" && cd "$1"
}

//Push to GCR
togcr() {
  local proj img
  proj="$(gcloud config get-value core/project -q)"
  img="europe-west2-docker.pkg.dev/$proj/myworks/$(basename "$1")"
  docker tag "$1" "$img"
  docker push "$img"
  echo >&2 "Pushed $(tput setaf 1)$img$(tput sgr0)"
}




//List gcp artifacts
gcr() {
    [ -n "$1" ] && [[ ! "$1" =~ ^.*-docker.pkg.dev ]] && set -- "$(gcloud config get-value artifacts/location -q)-docker.pkg.dev/$(gcloud config get-value core/project -q)/$1"
    
    c=$(echo "$1" | grep -o '/' | wc -l)
    if [[ $c -eq 1 ]]; then
        gcloud artifacts repositories list --filter="format=docker"
    elif [[ $c -eq 2 ]]; then
        gcloud artifacts docker images list "$1" --include-tags
    else
        gcloud artifacts repositories list
    fi
}



run() {
    img="$1"
    shift 1
    name="$1"
    if [[ -n "$name" ]]; then
        shift 1
    else
        name="$(basename "$img")"
    fi
    set -x
    gcloud run deploy --region europe-west2 --platform=managed \
        --allow-unauthenticated --image="$img" "$name" "$@"
}

//Build image and push to GCR
dbp() {
    proj="$(gcloud config get-value core/project -q)"
    if [[ -z $proj ]]; then
        echo "can't get project";
        return
    fi
    img="europe-west2-docker.pkg.dev/${proj}/myworks/$(basename $PWD)"
    echo >&2 "$(tput setaf 2)build+push $img $(tput sgr0)"
    (
    set -x
    docker build --tag "$img" ${1:-.} 1>/dev/null && \
        docker push "$img" 1>/dev/null && echo "$img"
    )
}

//Build image, push to GCR and deploy to cloud run
dbpr() {
    local img
    img="$(dbp)"
    echo >&2 "$(tput setaf 2)deploy $img to cloud run...$(tput sgr0)"
    run "$img" "$(basename "$img")" "$@"
}



