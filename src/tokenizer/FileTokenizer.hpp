#ifndef FILETOKENIZER_HPP
#define FILETOKENIZER_HPP

#include <fstream>
#include <stdexcept>
#include <vector>

#include "tokenizer/ITokenizer.hpp"

class	FileTokenizer: public ITokenizer
{
	class	FileTokenizerException;

	typedef std::pair<std::string, int>	t_tokenInfo;

// deleted
	FileTokenizer(const FileTokenizer& fileTokenizer);
	FileTokenizer	&operator=(const FileTokenizer& fileTokenizer);

public:
// constructors & destructor
	FileTokenizer();
	~FileTokenizer();

// member functions
	void				init(const std::string& path);

	virtual std::string	peek();
	virtual std::string	get();
	virtual bool		empty() const;
	void				eat(const std::string& target);
	unsigned int		size();

	std::string		getErrorLog(const char* message);
	void			printTokensByLine() const;
	void			printTokens() const;

private:
	void			tokenize(const std::string& chunk, int lineNumber);

// member variables
	std::fstream				m_fstream;
	std::vector<t_tokenInfo>	m_tokenArr;
	unsigned int				m_idx;

	static const std::string	s_eot;
};

#endif
