namespace ogl_examples
{

class ShaderCompiler
{
public:
	ShaderCompiler(const std::string& shader_src) = default;

	bool set_shader_source_file(const std::string& src);
	bool compile() const;
};


}