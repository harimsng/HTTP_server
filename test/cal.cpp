#include <queue>
#include <string>
#include <iostream>

struct	token
{
	enum	e_type
	{
		DIGIT = 1,
		OP1 = 2,
		OP2 = 3,
		PAREN = 4
	}
}

int		parse_expr(const std::queue<std::string>& tokenStream);
void	parse_term(const std::queue<std::string>& tokenStream);
int		parse_factor(const std::queue<std::string>& tokenStream);


int	main(int argc, char **argv)
{
	std::queue<std::string> tokenStream;

	for (int i = 1; i < argc; ++i)
		tokenStream.push(argv[i]);

	std::cout << "result = " << parse_expr(tokenStream) << '\n';
	return 0;
}

int	parse_expr(const std::queue<std::string>& tokenStream)
{
	int lhs = parse_term();
	while (peek().type == OP1)
	{
		rhs = parse_term();
	hoken = get();
		lhs = operate(token, rhs);
	}
	return lhs;
}

void	parse_term(const std::queue<std::string>& tokenStream)
{
	lhs = parse_factor();
	while (get().type == OP2)
	{
		rhs = parse_factor();
		lhs = lhs * rhs;
	}
	return lhs;
}

int		parse_factor(const std::queue<std::string>& tokenStream)
{
	if (peek().type == DECIMAL)
	{
		return get();
	}
	return parse_expr();
}
