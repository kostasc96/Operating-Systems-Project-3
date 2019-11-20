CC=gcc

all: restaurant doorman customer waiter


restaurant:
	$(CC) -o restaurant restaurant.c -lpthread -lrt

doorman:
	$(CC) -o doorman doorman.c -lpthread -lrt

customer:
	$(CC) -o customer customer.c -lpthread -lrt

waiter:
	$(CC) -o waiter waiter.c -lpthread -lrt



clean:
	rm -f restaurant
	rm -f doorman
	rm -f customer
	rm -f waiter

.PHONY: all clean
