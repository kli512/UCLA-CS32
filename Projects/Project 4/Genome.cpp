#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <algorithm> // is allowed or not?
using namespace std;

class GenomeImpl
{
public:
	GenomeImpl(const string& nm, const string& sequence);
	static bool load(istream& genomeSource, vector<Genome>& genomes);
	int length() const;
	string name() const;
	bool extract(int position, int length, string& fragment) const;
private:
	string m_name;
	string m_seq;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
	m_name = nm;
	m_seq = sequence;
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes)
{
	genomes.clear(); // mem leak? probably not

	string initial;
	getline(genomeSource, initial, '>');
	if (initial.length() != 0) {
		cerr << "file started with bad character" << endl;
		return false;
	}
	while (!genomeSource.eof()) {
		string nm;
		string raw_data;

		getline(genomeSource, nm, '\n');

		if (nm.length() == 0) {
			if (nm.length() == 0)
				cerr << "failed because len = " << nm.length() << endl;

			cerr << "line was:\n" << nm << endl;
			return false;
		}

		getline(genomeSource, raw_data, '>');
		string data;
		for (char c : raw_data) {
			switch (c) {
			case 'A':
			case 'C':
			case 'G':
			case 'T':
			case 'N':
				data += c;
				break;
			case 'a':
			case 'c':
			case 'g':
			case 't':
			case 'n':
				data += toupper(c);
				break;
			case '\n':
				break;
			default:
				cerr << "failed because of " << int(c) << endl;
				return false;
			}
		}
		genomes.push_back(Genome(nm, data));
	}
	return true;
}

int GenomeImpl::length() const
{
	return m_seq.length();
}

string GenomeImpl::name() const
{
	return m_name;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	if (length < 0 || position < 0 || position + length > m_seq.length()) // should it work when len == 0? currently does
		return false;
	fragment = m_seq.substr(position, length);
	return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
	m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
	delete m_impl;
}

Genome::Genome(const Genome& other)
{
	m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
	GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
	delete m_impl;
	m_impl = newImpl;
	return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
	return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
	return m_impl->length();
}

string Genome::name() const
{
	return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
	return m_impl->extract(position, length, fragment);
}