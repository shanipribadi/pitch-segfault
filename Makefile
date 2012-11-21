OBJDIR = obj
NAME = wsola

SRCS = main.cc jackclient.cc

LIBS = -ljack

CC = g++
LD = $(CC)

CFLAGS = -Wall -O2 -march=native -pipe -I.
OBJS = $(addprefix $(OBJDIR)/,$(SRCS:.cc=.o))
DEPS = $(addprefix $(OBJDIR)/,$(SRCS:.cc=.d))

all: $(OBJDIR) $(NAME)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.cc
	$(CC) -MMD $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(LD) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(OBJDIR)
	rm -f $(NAME)
