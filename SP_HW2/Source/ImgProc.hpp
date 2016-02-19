#pragma once
#include<opencv2\opencv.hpp>
#include <fstream>

#include<Performance.hpp>
#include<Common.hpp>
#define clip(x) (x<=255?x>=0?x:0:255)

class ImgProc
{
private:
	//config 
	bool isExist;
	int Mat_num;
	std::vector<std::string>Cmds;
	std::string input_path;
	std::string output_path;
	std::string filter_path;
	//Image container
	std::vector<cv::Mat> Matvec;
public:
	ImgProc(std::string config_path_in);
	bool IsExist();
	int MatNum();
	void print_cmd();
	void main_flow();
	int read_grayscale(std::vector<std::string> splitted_cmd);
	int show_img(std::vector<std::string> splitted_cmd);
	int box_filter(std::vector<std::string> splitted_cmd);
	void Convolution2D();

};
ImgProc::ImgProc(std::string config_path_in){
	std::fstream config_file;
	std::cout << config_path_in << std::endl;
	config_file.open(config_path_in.c_str(), std::ios::in);
	if (config_file.is_open()){
		isExist = true;
		std::cout << "%% Parsing config file \" " << config_path_in << "\"" << std::endl;
		std::string raw_line;
		while (getline(config_file, raw_line)){
			std::vector<std::string> splitted_string = Token(raw_line, ',');
			if (splitted_string.size() > 0){
				if (splitted_string[0] == "#")
					std::cout << raw_line << std::endl;
				else{
					if (splitted_string[0] == "input_path"){
						input_path = splitted_string[1];
					}
					else if (splitted_string[0] == "output_path"){
						output_path = splitted_string[1];
					}
					else if (splitted_string[0] == "filter_path"){
						filter_path = splitted_string[1];
					}
					else if (splitted_string[0] == "Mat_num"){
						Mat_num = ConvertFromString<int>(splitted_string[1]);
					}
					else if (splitted_string[0] == "Command:"){
						while (getline(config_file, raw_line)){
							if (splitted_string[0] == "#")
								std::cout << raw_line << std::endl;
							else
								Cmds.push_back(raw_line);
						}
					}
				}
			}
		}
	}
	else{
		isExist = false;
		std::cout << "%% Error : config file doesn't exist." << std::endl;
	}
	if (Cmds.size() == 0)
		isExist = false;
}

bool ImgProc::IsExist(){
	return isExist;
}
int ImgProc::MatNum(){
	return Mat_num;
}
void ImgProc::print_cmd(){
	std::cout << Cmds.size() << std::endl;
	for (int i = 0; i < Cmds.size(); i++)
		std::cout << Cmds[i] << std::endl;
}


int ImgProc::read_grayscale(std::vector<std::string> splitted_cmd){
	if (splitted_cmd.size() != 3){
		std::cout << "%% Error: Wrong cmd format" << std::endl;
		return 0;
	}
	int object = ConvertFromString<int>(splitted_cmd[1]);
	if (object >= Mat_num){
		std::cout << "%% Error: Object doesn't exist" << std::endl;
		return 0;
	}
	Matvec.at(object) = cv::imread(input_path + splitted_cmd[2], cv::IMREAD_GRAYSCALE);
	if (Matvec[object].cols<1 || Matvec[object].rows<1){
		std::cout << "%% Error: Read image fails" << std::endl;
		return 0;
	}
	return 1;
}
int ImgProc::show_img(std::vector<std::string> splitted_cmd){
	if (splitted_cmd.size() != 3){
		std::cout << "%% Error: Wrong cmd format" << std::endl;
		return 0;
	}
	int object = ConvertFromString<int>(splitted_cmd[1]);
	if (object >= Mat_num){
		std::cout << "%% Error: Object doesn't exist" << std::endl;
		return 0;
	}
	if (Matvec[object].cols<1 || Matvec[object].rows<1){
		std::cout << "%% Error: Image does not exist" << std::endl;
		return 0;
	}
	cv::imshow(splitted_cmd[2], Matvec[object]);
	cvWaitKey(10);
	return 1;
}
int ImgProc::box_filter(std::vector<std::string> splitted_cmd){
	if (splitted_cmd.size() != 4){
		std::cout << "%% Error: Wrong cmd format" << std::endl;
		return 0;
	}
	int subject = ConvertFromString<int>(splitted_cmd[1]);
	int object = ConvertFromString<int>(splitted_cmd[2]);
	int size = ConvertFromString<int>(splitted_cmd[3]);
	if (object >= Mat_num || subject >= Mat_num){
		std::cout << "%% Error: Object/Subject doesn't exist" << std::endl;
		return 0;
	}
	int step = (size + 1) / 2;
	cv::Mat pad;
	padding(&Matvec[subject], &pad, step);
	Matvec[object] = cv::Mat(Matvec[subject].size(), Matvec[subject].type());
	uchar *pad_ptr, *obj_ptr;
	int sum, num = size*size;
	for (int i = 0; i < Matvec[object].rows; i++){
		obj_ptr = Matvec[object].ptr(i);
		for (int j = 0; j < Matvec[object].cols; j++){
			sum = 0;
			for (int ix = -step; ix <= step; ix++){
				pad_ptr = pad.ptr(i + ix + step, j);
				for (int jx = -step; jx <= step; jx++){
					sum += *pad_ptr++;
				}
			}
			*obj_ptr++ = int(clip(sum / num) + 0.5);
		}
	}
	return 1;
}

void ImgProc::main_flow(){
	//Create Mat objects
	cv::Mat A;
	Performance_Record PR;
	for (int i = 0; i < Mat_num; i++){
		Matvec.push_back(A);
	}
	for (int i = 0; i < Cmds.size(); i++){
		//parse cmd
		std::cout << "%% " << Cmds[i] << " ..." << std::endl;
		std::vector<std::string> splitted_cmd = Token(Cmds[i], ',');
		if (splitted_cmd[0] == "read_grayscale"){
			if (!read_grayscale(splitted_cmd))
				break;
		}
		else if (splitted_cmd[0] == "show_img"){
			if (!show_img(splitted_cmd))
				break;
		}
		else if (splitted_cmd[0] == "box_filter"){
			PR.set_start();
			int flag = box_filter(splitted_cmd);
			PR.set_end();
		}
			
	}
	cv::waitKey(0);

}