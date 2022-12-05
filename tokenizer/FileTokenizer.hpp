#ifndef FILETOKENIZER_HPP
#define FILETOKENIZER_HPP

#include <sys/stat.h>

#include <fstream>
#include <stdexcept>

#include "ATokenizer.hpp"

using namespace std;

class	FileTokenizer: public ATokenizer
{
	enum e_tokenType {
		WORD = 1,
		SPECIAL
	};

	// need token struct?
	// struct token {
	//     e_tokenType type;
	//     string		token;
	// };
// deleted
	FileTokenizer(FileTokenizer const& fileTokenizer);
	FileTokenizer	&operator=(FileTokenizer const& fileTokenizer);

public:
// constructors & destructor
	FileTokenizer();
	~FileTokenizer();

// member functions
	void					init(const string& path);

	virtual const string&	peek();
	virtual const string&	get();
	virtual bool			empty() const;

private:
	string&			getErrorLog();
	void			tokenize(const string& chunk);
	bool			isValidFileName(const string& path);

// member variables
	vector<string>	m_tokenArr;
	unsigned int	m_idx;
};

#endif
