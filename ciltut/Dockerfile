# Use an official OCaml base image
FROM ocaml/opam:alpine-ocaml-4.02

# Switch to root user to install dependencies
USER root

# Update the package list and install necessary packages
RUN apk update && apk add --no-cache \
    build-base \
    curl \
    git \
    perl

# Manually change the shell for the opam user to /bin/sh
RUN sed -i 's#/sbin/nologin#/bin/sh#' /etc/passwd

# Switch back to the opam user
USER opam

# Initialize OPAM
RUN opam init -y --compiler=4.01.0 && \
    opam switch create 4.01.0 && \
    eval $(opam env)

# Install OCaml and OPAM
RUN opam install -y ocaml ocamlfind cil

# Set the working directory
WORKDIR /home/opam/app

# Set the default command to run when starting the container
CMD ["ocaml"]