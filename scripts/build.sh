#!/bin/bash

premake5 gmake
make -C generated/ $@
