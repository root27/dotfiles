setopt aliases


alias gcloud="~/google-cloud-sdk/bin/gcloud"

mcd() {
  mkdir -p "${1:?Need to specify an argument}" && cd "$1"
}

togcr() {
  local proj img
  proj="$(gcloud config get-value core/project -q)"
  img="europe-west2-docker.pkg.dev/$proj/myworks/$(basename "$1")"
  docker tag "$1" "$img"
  docker push "$img"
  echo >&2 "Pushed $(tput setaf 1)$img$(tput sgr0)"
}




delgcr() {


    if [[ -z "$1" ]]; then
        echo "Usage: delgcr <image-name>"
        return
    fi

    gcloud artifacts docker images delete "$1" 

}

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
    docker build ${1:---platform=linux/amd64} --tag "$img" ${2:-.} 1>/dev/null && \
        docker push "$img" 1>/dev/null && echo "$img"
    )
}

dbpr() {
    local img
    img="$(dbp)"
    echo >&2 "$(tput setaf 2)deploy $img to cloud run...$(tput sgr0)"
    run "$img" "$(basename "$img")" "$@"
}


listrun() {
    
    if [[ -z "$1" ]]; then
        gcloud run services list
        return
    fi

    gcloud run services list --platform=managed ${1}
}


delrun() {
    if [[ -z "$1" ]]; then
        echo "Usage: delrun <service-name>"
        return
    fi
    gcloud run services delete "$1" --platform=managed ${2:---region=europe-west2}
}



