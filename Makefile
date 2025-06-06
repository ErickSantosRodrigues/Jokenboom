CC := gcc
SRCDIR := src
BUILDDIR := build
BINDIR := bin

SRCEXT := c
INC := -I include/ -I third_party/

SERVER_SRC := $(SRCDIR)/server.$(SRCEXT) $(SRCDIR)/game.$(SRCEXT)
CLIENT_SRC := $(SRCDIR)/client.$(SRCEXT) $(SRCDIR)/game.$(SRCEXT)
GAME_SRC   := $(SRCDIR)/game.$(SRCEXT)

SERVER_OBJ := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SERVER_SRC:.$(SRCEXT)=.o))
CLIENT_OBJ := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CLIENT_SRC:.$(SRCEXT)=.o))
GAME_OBJ   := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(GAME_SRC:.$(SRCEXT)=.o))

TARGETS := $(BINDIR)/server $(BINDIR)/client 

all: $(TARGETS)

$(BINDIR)/server: $(SERVER_OBJ)
	@mkdir -p $(@D)
	$(CC) $^ -o $@

$(BINDIR)/client: $(CLIENT_OBJ)
	@mkdir -p $(@D)
	$(CC) $^ -o $@


$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(@D)
	$(CC) $(INC) -c -o $@ $<

clean:
	$(RM) -r $(BUILDDIR)/* $(TARGETS)

.PHONY: all clean
