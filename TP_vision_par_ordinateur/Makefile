all:final

final:main.o gradient.o k_means.o germ.o low_filter.o hough_transformation.o laplacien.o operations.o
	gcc main.o utilitaire.o gradient.o matrix.o operations.o  thresholding.o k_means.o germ.o laplacien.o low_filter.o convolution.o linked_list.o stack.o image.o -lm -o main

main.o:main.c 
	gcc -c -w main.c -o main.o

utilitaire.o: source/utilitaire/utilitaire.c header/utilitaire/utilitaire.h header/struct/pixel.h header/struct/Image.h header/struct/matrix.h header/struct/structure.h
	gcc -c -w source/utilitaire/utilitaire.c -o utilitaire.o

# utilitaire.h: header/utilitaire/utilitaire.h header/struct/pixel.h header/struct/Image.h header/struct/matrix.h header/struct/structure.h

convolution.o: source/convolution/convolution.c  matrix.o utilitaire.o header/convolution/convolution.h 
	gcc -c -w source/convolution/convolution.c -o convolution.o

# convolution.h: header/convolution/convolution.h header/struct/matrix.h 

operations.o: source/base_operations/operations.c image.o utilitaire.o matrix.o header/base_operations/operations.h  
	gcc -c -w source/base_operations/operations.c -o operations.o

# operations.h: header/base_operations/operations.h header/struct/Image.h header/struct/matrix.h 

matrix.o: source/struct/matrix.c header/struct/matrix.h
	gcc -c -w source/struct/matrix.c -o matrix.o

# matrix.h: header/struct/matrix.h

k_means.o: source/segmentation/k_means.c linked_list.o utilitaire.o image.o header/segmentation/k_means.h header/struct/structure.h 
	gcc -c -w source/segmentation/k_means.c  -o k_means.o

# k_means.h: header/segmentation/k_means.h header/struct/Image.h header/data_structure/linked_list.h header/struct/structure.h 

germ.o: source/segmentation/germ.c linked_list.o stack.o utilitaire.o image.o stack.o header/segmentation/germ.h header/struct/structure.h 
	gcc -c -W source/segmentation/germ.c -o germ.o

gradient.o: source/filter/high_pass/gradient.c thresholding.o convolution.o matrix.o image.o utilitaire.o header/filter/high_pass/gradient.h 
	gcc -c -w source/filter/high_pass/gradient.c -o gradient.o

# gradient.h:header/filter/high_pass/gradient.h header/struct/Image.h header/struct/matrix.h 

laplacien.o: source/filter/high_pass/laplacien.c thresholding.o convolution.o image.o matrix.o header/filter/high_pass/laplacien.h   
	gcc -c -w source/filter/high_pass/laplacien.c -o laplacien.o

# laplacien.h: header/filter/high_pass/laplacien.h header/struct/Image.h header/struct/matrix.h 

low_filter.o: source/filter/low_pass/low_filter.c image.o convolution.o utilitaire.o header/filter/low_pass/low_filter.h 
	gcc -c -w source/filter/low_pass/low_filter.c -o low_filter.o

# low_filter.h: header/filter/low_pass/low_filter.h header/struct/Image.h

hough_transformation.o: source/transformations/hough_transformation.c  utilitaire.o image.o gradient.o laplacien.o linked_list.o header/transformations/hough_transformation.h
	gcc -c -W source/transformations/hough_transformation.c -o hough_transformation.o

# hough_transformation.h:header/transformations/hough_transformation.h header/struct/Image.h header/struct/matrix.h header/struct/structure.h

linked_list.o: source/data_structure/linked_list.c  header/data_structure/linked_list.h header/struct/structure.h
	gcc -c -W source/data_structure/linked_list.c -o linked_list.o

# linked_list.h: header/data_structure/linked_list.h header/struct/structure.h

stack.o: source/data_structure/stack.c header/data_structure/stack.h header/struct/structure.h
	gcc -c -w source/data_structure/stack.c  -o stack.o

# stack.h:header/data_structure/stack.h

thresholding.o: source/segmentation/thresholding.c image.o matrix.o operations.o utilitaire.o header/segmentation/thresholding.h
	gcc -c -w source/segmentation/thresholding.c  -o thresholding.o
  # thresholding.h:header/segmentation/thresholding.h header/struct/Image.h header/struct/matrix.h 

image.o: source/struct/Image.c header/struct/Image.h
	gcc -c -w source/struct/Image.c -o image.o

clean:
	rm -rf *.o
mrproper:clean
	rm -rf main

# image.h: header/struct/Image.h