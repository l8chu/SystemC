#include "systemc.h"
#include "functions.h"
#include "fdct.h"
#include "idct.h"

#define NS *1e-9 // use this constant to make sure that the clock signal is in nanoseconds

int sc_main(int argc, char *argv[]) {
	char choice;
	sc_signal<FILE *> sc_input; 	// input file pointer signal
	sc_signal<FILE *> sc_output;	// output file pointer signal
	sc_signal<double> dct_data[8][8]; // signal to the dc transformed data
	sc_signal<double> cosine_tbl[8][8]; // signal for the cosine table values

	sc_signal<bool> clk1, clk2; 		// clock signal for FDCT and IDCT, respectively

	FILE *input, *output; // input and output file pointers
	double cosine[8][8]; // the cosine table
	double data[8][8]; // the data read from the signals, which will be zigzagged

	if (argc == 4) {
		if (!(input = fopen(argv[1], "rb")))
			// some error occurred while trying to open file
			// print an error message and exit.
			printf("\nSystemC JPEG LAB:\ncannot open file '%s'\n", argv[1]), exit(1);
		if (!(output = fopen(argv[2], "wb")))
			// some error occurred while trying to create file
			// print an error message and exit.
			printf("\nSystemC JPEG LAB:\ncannot create file '%s'\n", argv[2]), exit(1);
		// copy the input and output file pointer onto the respective ports
		sc_input.write(input);
		sc_output.write(output);
		choice = *argv[3];
	} else
		fprintf(stderr, "\nSystemC JPEG LAB: too many or few command line arguments\n"
			"usage:  ./sc_jpeg.x [input file] [output file] [(C)ompress or (D)ecompress]\n")
				, exit(1);

	write_read_header(input, output); // write the header read from the input file
	make_cosine_tbl(cosine); // make the cosine table

	// copy the cosine table and the quantization table onto the corresponding signals
	for (unsigned char i = 0; i < 8; i++)
		for (unsigned char j = 0; j < 8; j++)
			cosine_tbl[i][j].write(cosine[i][j]);

	fdct IDCT("idct"); // call the forward discrete transform module
	// bind the ports
	for (unsigned char i = 0; i < 8; i++)
		for (unsigned char j = 0; j < 8; j++) {
			IDCT.out64[i][j](dct_data[i][j]);
			IDCT.icosine[i][j](cosine_tbl[i][j]);
		}
	IDCT.clk(clk2);
	IDCT.sc_input(sc_input);



	// because compression and decompression are two different processes, we must use
	// two different clocks, to make sure that when we want to compress, we only compress
	// and dont decompress by mistake
	sc_start(SC_ZERO_TIME); 	// initialize the clock
	if ((choice == 'c') || (choice == 'C')) { // for compression
		while (!(feof(input))) {
			// create the FDCT clock signal
			clk1.write(1);		// convert the clock to high
			sc_start(10, SC_NS);	// cycle the high for 10 nanoseconds
			clk1.write(0);		// start the clock as low
			sc_start(10, SC_NS);	// cycle the low for 10 nanoseconds

			// read all the signals into the data variable, so we can use those values
			// in a software block
			for (unsigned char i = 0; i < 8; i++)
				for (unsigned char j = 0; j < 8; j++)
					data[i][j] = dct_data[i][j].read();
			zigzag_quant(data, output); // zigzag and quantize the read data
		}

	} else if ((choice == 'd') || (choice == 'D')) { // for decompression

		printf("This is your job to complete.\n");
        while(!(feof(input))){
            clk2.write(1);
            sc_start(10,SC_NS);
            clk2.write(0);
            sc_start(10,SC_NS);
            
            for(unsigned char i=0;i<8;i++)
                for(unsigned char j=0;j<8;j++)
                    data[i][j] = dct_data[i][j].read();
            unzigzag_iquant(data,output);
        }
	}	// cycle the clock for as long as there is something to be read from the input file

	fclose(sc_input.read());
	fclose(sc_output.read());

	return 0;
}
