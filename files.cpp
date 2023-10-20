#include <filesystem>
#include <fstream>
#include <vector>

/*
 * File's interface
 */
class File {
 public:
  File() = default;

  File(const File&) = delete;
  File& operator=(const File&) = delete;

  File(File&& other):
    path_(std::move(other.path_)),
    content_(std::move(other.content_)),
    hash_(other.hash_) {}

  File& operator=(File&& other) {
    path_ = std::move(other.path_);
    content_ = std::move(other.content_);
    hash_ = other.hash_;
    return *this;
  }

  File(const std::string& path): path_(path) {
    std::ifstream ifs(path, std::ios::binary);
    content_ = std::move(std::string{
      std::istreambuf_iterator<char>{ifs},
      std::istreambuf_iterator<char>{}
    });

    hash_ = std::hash<std::string>{}(content_);
  }

  ~File() = default;

  const std::string& GetPath() const noexcept {
    return path_;
  }

  const std::string& GetContent() const noexcept {
    return content_;
  }

  size_t GetHash() const noexcept {
    return hash_;
  }

 private:
  std::string path_;
  std::string content_;
  size_t hash_{0};
};
// class File


std::vector<File> GetDirectoryFiles(std::string dirPath) {
  std::vector<File> result;
  result.reserve(20);

  for (auto& entry : std::filesystem::directory_iterator(dirPath)) {
    result.emplace_back(entry.path());
  }

  return result; // copy elision
}
