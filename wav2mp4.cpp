#include <algorithm>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <system_error>

#include "wave/file.h"

const int fps = 30;
const int width = 640;
const int height = 480;
const cv::Vec3b col(0x32, 0xcd, 0x32); // color of the waveform

void draw_wav(cv::Mat &img, std::vector<float> dat) {
  auto s = img.size();
  auto w = s.width;
  auto h = s.height;

  auto dsize = dat.size();

  for (int i = 0; i < dsize/2; i++) {
    float d = (dat[i*2] + dat[i*2+1])/2.0;   // Averaging stereo data

    int x = int(i *2* w / dsize);
    int y = int(h * 3 / 4) + int(d * h / 4);

    if (x > w) {
      x = w;
    }
    if (x < 0) {
      x = 0;
    }
    if (y > h) {
      y = h;
    }
    if (y < 0) {
      y = 0;
    }

    img.at<cv::Vec3b>(y, x) = col;
  }
}

int main(int argc, char *argv[]) {
  wave::File read_file;
  if (argc < 2) {
    std::cout << "Usage: wav2mp4 target.wav" << std::endl;
    return -1;
  }
  std::string target_wav = argv[1];
  std::string target_movie = target_wav + ".mp4";

  wave::Error err = read_file.Open(target_wav, wave::kIn);
  if (err) {
    std::cout << "can't find target wav file" << std::endl;
    return -1;
  }

  auto sample_rate = read_file.sample_rate();

  std::vector<float> content;
  err = read_file.Read(&content);
  if (err) {
    std::cout << "Something went wrong in read" << std::endl;
    return -1;
  }

  cv::VideoWriter writer;

  auto fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
  writer.open(target_movie, fourcc, fps, cv::Size(width, height));

  int chunk_size = int(sample_rate / fps);

  std::vector<float> chunk;
  int index = 0;

  for (int i = 0; i < content.size()/2 / chunk_size; i++) {
    int index = i * chunk_size*2;

    auto last_index = index + chunk_size;
    if(last_index<content.size()){
      chunk.assign(&content[index], &content[last_index]);
    }

    cv::Mat img = cv::Mat::zeros(height, width, CV_8UC3);
    draw_wav(img, chunk);

    writer << img;
    std::cout << "\r" << std::setw(10) << i << " / "
              << content.size() / chunk_size;
  }
  std::cout << std::endl;
  return 0;
}
