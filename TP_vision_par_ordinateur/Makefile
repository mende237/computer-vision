final:main.o 
	gcc objects/main.o objects/utilitaire.o objects/gradient.o objects/matrix.o objects/operations.o  objects/thresholding.o objects/k_means.o objects/germ.o objects/laplacien.o objects/low_filter.o objects/convolution.o objects/linked_list.o objects/stack.o objects/image.o -lm -o objects/main

main.o:main.c gradient.o k_means.o germ.o low_filter.o hough_transformation.o laplacien.o operations.o
	gcc -c -w main.c -o objects/main.o

utilitaire.o: source/utilitaire/utilitaire.c utilitaire.h 
	gcc -c -w source/utilitaire/utilitaire.c -o objects/utilitaire.o

utilitaire.h: header/utilitaire/utilitaire.h header/struct/pixel.h header/struct/Image.h header/struct/matrix.h header/struct/structure.h

convolution.o: source/convolution/convolution.c convolution.h matrix.o utilitaire.o 
	gcc -c -w source/convolution/convolution.c -o objects/convolution.o

convolution.h: header/convolution/convolution.h header/struct/matrix.h 

operations.o: source/base_operations/operations.c  operations.h image.o utilitaire.o
	gcc -c -w source/base_operations/operations.c -o objects/operations.o

operations.h: header/base_operations/operations.h header/struct/Image.h header/struct/matrix.h 

matrix.o: source/struct/matrix.c matrix.h
	gcc -c -w source/struct/matrix.c -o objects/matrix.o

matrix.h: header/struct/matrix.h

k_means.o: source/segmentation/k_means.c k_means.h linked_list.o utilitaire.o image.o
	gcc -c -w source/segmentation/k_means.c  -o objects/k_means.o

k_means.h: header/segmentation/k_means.h header/struct/Image.h header/data_structure/linked_list.h header/struct/structure.h 

germ.o: source/segmentation/germ.c germ.h linked_list.o stack.o utilitaire.o image.o stack.o
	gcc -c -W source/segmentation/germ.c -o objects/germ.o

germ.h: header/segmentation/germ.h header/struct/Image.h header/data_structure/linked_list.h header/struct/structure.h 

gradient.o: source/filter/high_pass/gradient.c gradient.h thresholding.o convolution.o matrix.o image.o utilitaire.o
	gcc -c -w source/filter/high_pass/gradient.c -o objects/gradient.o

gradient.h:header/filter/high_pass/gradient.h header/struct/Image.h header/struct/matrix.h 

laplacien.o: source/filter/high_pass/laplacien.c laplacien.h thresholding.o convolution.o image.o
	gcc -c -w source/filter/high_pass/laplacien.c -o objects/laplacien.o

laplacien.h: header/filter/high_pass/laplacien.h header/struct/Image.h header/struct/matrix.h 

low_filter.o: source/filter/low_pass/low_filter.c low_filter.h image.o convolution.o utilitaire.o
	gcc -c -w source/filter/low_pass/low_filter.c -o objects/low_filter.o

low_filter.h: header/filter/low_pass/low_filter.h header/struct/Image.h

hough_transformation.o: source/transformations/hough_transformation.c hough_transformation.h
	gcc -c -lm -W source/transformations/hough_transformation.c -o objects/hough_transformation.o

hough_transformation.h:header/transformations/hough_transformation.h header/struct/Image.h header/struct/matrix.h header/struct/structure.h

linked_list.o: source/data_structure/linked_list.c linked_list.h 
	gcc -c -W source/data_structure/linked_list.c -o objects/linked_list.o

linked_list.h: header/data_structure/linked_list.h header/struct/structure.h

stack.o: source/data_structure/stack.c header/data_structure/stack.h
	gcc -c -w source/data_structure/stack.c  -o objects/stack.o

stack.h:header/data_structure/stack.h

thresholding.o: source/segmentation/thresholding.c thresholding.h image.o matrix.o operations.o utilitaire.o
	gcc -c -w source/segmentation/thresholding.c  -o objects/thresholding.o

thresholding.h:header/segmentation/thresholding.h header/struct/Image.h header/struct/matrix.h 

image.o: source/struct/Image.c image.h
	gcc -c -w source/struct/Image.c -o objects/image.o

image.h: header/struct/Image.h

