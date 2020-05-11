#!/usr/bin/env bash

for i in {1..1000}
do
  ./output-client p $i $i
done
