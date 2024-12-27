flags = -std=c++11 -Wall -Wextra -Werror -pedantic

all: person user driver

person: person.cpp
	g++ person.cpp -o person $(flags)

user: user.cpp
	g++ user.cpp -o user $(flags)

driver: driver.cpp
	g++ driver.cpp -o driver $(flags)

clean:
	rm person user driver