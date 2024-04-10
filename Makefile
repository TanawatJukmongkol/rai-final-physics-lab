# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tjukmong <tjukmong@student.42bangkok.com>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/10 04:56:45 by tjukmong          #+#    #+#              #
#    Updated: 2024/04/10 15:28:32 by tjukmong         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Basic configs
NAME			= supun-final

# Sources and headers

MAIN_SRC		= $(NAME).ino
SRCS			= $(MAIN_SRC) \
				  alarm.ino
HEADERS			= alarm.hpp \
				  pitches.hpp \
				  supun-final.hpp.sample
LIBRARIES		=

# Path configuration
SRC_DIR			= .
HEADER_DIR		= .
LIBRARY_DIR		= library

# Internal environment variables
MAIN			= $(addprefix $(SRC_DIR)/,$(MAIN_SRC))
SRC				= $(addprefix $(SRC_DIR)/,$(SRCS))
HEADER			= $(addprefix $(HEADER_DIR)/,$(HEADERS))
LIBRARY			= $(addprefix $(LIBRARY_DIR)/,$(LIBRARIES))
SPACE			:= $(subst ,, )

$(NAME): $(MAIN) .git .gitignore

$(MAIN):
	@ echo "void setup() {" > $(MAIN)
	@ echo "  // put your setup code here, to run once:" >> $(MAIN)
	@ echo -ne "}\n\n" >> $(MAIN)
	@ echo "void loop() {" >> $(MAIN)
	@ echo "  // put your main code here, to run repeatedly:" >> $(MAIN)
	@ echo "}" >> $(MAIN)

%.class:
	@ echo "Generating class header boilerplate..."
	@ $(eval class_name:=$(subst .class,,$@))
	@ $(eval class_header:=$(HEADER_DIR)/$(class_name).hpp)
	@ $(eval class_header_def:=$(shell echo $(class_name)_HPP | tr '[:lower:]' '[:upper:]'))
	@ echo "#ifndef $(class_header_def)" > $(class_header)
	@ echo -ne "# define $(class_header_def)\n\n" >> $(class_header)
	@ echo -ne "class $(class_name)\n{\n\tpublic:\n" >> $(class_header)
	@ echo -ne "\t\t$(class_name)();\n\t\t~$(class_name)();\n" >> $(class_header)
	@ echo -ne "\t\t$(class_name)($(class_name) const &copy);\n" >> $(class_header)
	@ echo -ne "\t\t$(class_name) &operator=($(class_name) const &assignment);\n" >> $(class_header)
	@ echo -ne "};\n\n#endif\n" >> $(class_header)
	@ echo "Generating class body boilerplate..."
	@ $(eval class_body:=$(SRC_DIR)/$(class_name).ino)
	@ echo -ne "#include \"$(class_header)\"\n\n" > $(class_body)
	@ echo -ne "$(class_name)::$(class_name)()\n{\n}\n\n" >> $(class_body)
	@ echo -ne "$(class_name)::~$(class_name)()\n{\n}\n\n" >> $(class_body)
	@ echo -ne "$(class_name)::$(class_name)($(class_name) const &copy)\n{\n\t*this = copy;\n}\n\n" >> $(class_body)
	@ echo -ne "$(class_name)\t&$(class_name)::operator=($(class_name) const &assignment)\n{\n\treturn *this;\n}\n" >> $(class_body)

%.class-rm:
	@ $(eval class_name:=$(subst .class-rm,,$@))
	@ $(eval class_header:=$(HEADER_DIR)/$(class_name).hpp)
	@ $(eval class_body:=$(SRC_DIR)/$(class_name).ino)
	@ echo "Remove class $(class_name)..."
	@ rm -f $(class_header) $(class_body)

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.ino $(HEADER) Makefile
	@ echo "CXX $<"
	@ $(CXX) $(CXXFLAGS) $(INCLUDES_OBJ) -c $< -o $@

.git:
	git init

.gitignore: Makefile
	@ echo "Generate .gitignore..."
	@ echo -ne "*\n!*/\n!.gitignore\n!Makefile\n" > .gitignore
	@ echo -ne "$(subst ./,,$(subst $(SPACE),\n,$(addprefix !,${SRC})))\n" >> .gitignore
	@ echo -ne "$(subst ./,,$(subst $(SPACE),\n,$(addprefix !,${HEADER})))\n" >> .gitignore
	@ echo -ne "$(subst ./,,$(subst $(SPACE),\n,$(addprefix !,${LIBRARY})))\n" >> .gitignore
	@ echo "Rebuild tracked files cache..."
	@- git rm -rf --cached . 2> /dev/null
	@ git add .

.PHONY: all %.class
