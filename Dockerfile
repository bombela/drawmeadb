FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
		libexpat1 \
		libltdl7 \
		libssl3 \
		libcairo2 \
		libpango1.0-0 \
		ca-certificates \
		&& rm -rf /var/lib/apt/lists/*
COPY . /
EXPOSE 80
CMD ["./websrv"]
