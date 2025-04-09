TYPES ?= activities classes component deployment sequence states usecase
PUML ?= $(foreach type, $(TYPES), $(wildcard $(type).d/*.puml))
PNG ?= $(PUML:.puml=.png)

PLANTUML_CMD ?= plantuml
RM_CMD ?= rm -f

.PHONY: all
all: compile

.PHONY: compile
compile: $(PNG)

.PHONY:
clean:
	$(RM_CMD) $(PNG)

%.png: %.puml
	$(PLANTUML_CMD) $<
