#!/bin/bash

export PATH=/mnt/opt.net/application/ansys/15.0/v150/CFX/bin:$PATH
export PATH=/mnt/opt.net/application/ansys/15.0/v150/TurboGrid/bin:$PATH

cfxtg -batch turboGridSession.tse -v
