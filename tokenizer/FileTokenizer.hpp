#ifndef FILETOKENIZER_HPP
#define FILETOKENIZER_HPP

#include <fstream>
#include <stdexcept>

#include "ATokenizer.hpp"

class	FileTokenizer: public ATokenizer
{
	enum e_tokenType {
		WORD = 1,
		SPECIAL
	};
// deleted
	FileTokenizer(FileTokenizer const& fileTokenizer);
	FileTokenizer	&operator=(FileTokenizer const& fileTokenizer);

public:
// constructors & destructor
	FileTokenizer();
	~FileTokenizer();

// member functions
	void				init(std::string const path);

	virtual std::string const&	peek();
	virtual std::string const&	get();
	virtual bool				empty() const;

private:
	std::string&	getErrorLog();
	void			tokenize(const std::string& chunk);

// member variables
	std::vector<std::string>	m_tokenArr;
	unsigned int				m_idx;
};

#endif
