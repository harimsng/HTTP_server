#ifndef FILETOKENIZER_HPP
#define FILETOKENIZER_HPP

#include <fstream>
#include <stdexcept>

#include "ATokenizer.hpp"

class	FileTokenizer: public ATokenizer
{
	class	FileTokenizerException;

	typedef std::pair<std::string, int>	t_tokenInfo;

// deleted
	FileTokenizer(FileTokenizer const& fileTokenizer);
	FileTokenizer	&operator=(FileTokenizer const& fileTokenizer);

public:
// constructors & destructor
	FileTokenizer();
	~FileTokenizer();

// member functions
	void				init(std::string const path);

	virtual const std::string&	peek();
	virtual const std::string&	get();
	virtual bool				empty() const;
	void						eat(const std::string& target);
	unsigned int				size();

	std::string		getErrorLog(int idx);
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
