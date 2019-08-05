#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <set>
#include <unordered_map>

using namespace std;

class GenomeMatcherImpl
{
public:
	GenomeMatcherImpl(int minSearchLength);
	void addGenome(const Genome& genome);
	int minimumSearchLength() const;
	bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
	bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	struct GenomeFrag {
		GenomeFrag(const int pos, const Genome* g) :
			m_pos(pos), m_g(g) {};

		int m_pos;
		const Genome* m_g;
	};

	Trie<GenomeFrag> m_trie;
	list<Genome> m_genomes;
	int minSL;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
	minSL = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
	if (genome.length() < minSL) {
		return;
	}
	m_genomes.push_back(genome);
	Genome& ngenome = m_genomes.back();
	for (int i = 0; i < ngenome.length() - minSL + 1; i++) { // check for off by 1 error
		string frag;
		ngenome.extract(i, minSL, frag);
		m_trie.insert(frag, GenomeFrag(i, &ngenome));
		//cerr << "added genome at " << frag << " in tree, genome location is " << &ngenome << endl;
	}
}

int GenomeMatcherImpl::minimumSearchLength() const
{
	return minSL;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	if (fragment.length() < minimumLength || minimumLength < minSL)
		return false;

	string fragKey = fragment.substr(0, minSL);

	vector<GenomeFrag> gf_matches = m_trie.find(fragKey, exactMatchOnly);

	/*cerr << "found " << gf_matches.size() << " matches" << endl;
	for (GenomeFrag gf : gf_matches) {
		cerr << "found in " << gf.m_g->name() << endl;
		cerr << "at pos " << gf.m_pos << endl;
	}*/

	unordered_map<const Genome*, DNAMatch> matchHolder;

	for (GenomeFrag gf : gf_matches) {
		string toCheck;
		int toCheck_l;
		for (toCheck_l = fragment.length(); toCheck_l >= minimumLength; toCheck_l--) {
			if (gf.m_g->extract(gf.m_pos, toCheck_l, toCheck))
				break;
		}
		if (toCheck.length() < minimumLength) {
			continue;
		}

		int matchingLen;

		if (exactMatchOnly) {
			for (matchingLen = minSL; matchingLen < toCheck_l; matchingLen++) {
				if (toCheck[matchingLen] != fragment[matchingLen])
					break;
			}
		}
		else {
			bool failed = false;
			for (int i = 0; i < minSL; i++) {
				if (toCheck[i] != fragment[i]) {
					failed = true;
					break; // if find works this should be fine
				}
			}
			for (matchingLen = minSL; matchingLen < toCheck_l; matchingLen++) {
				if (toCheck[matchingLen] != fragment[matchingLen]) {
					if (failed)
						break;
					failed = true;
				}
			}
		}

		//cerr << "matched up to " << matchingLen << " characters with genome " << gf.m_g->name() << " at pos " << gf.m_pos << endl;

		if (matchingLen < minimumLength) {
			//cerr << "not a long enough match " << endl;
			continue;
		}

		DNAMatch match;
		match.genomeName = gf.m_g->name();
		match.length = matchingLen;
		match.position = gf.m_pos;

		if (matchHolder.find(gf.m_g) != matchHolder.end())
			matchHolder[gf.m_g] = (matchHolder[gf.m_g].length >= match.length) ? matchHolder[gf.m_g] : match;
		else
			matchHolder[gf.m_g] = match;
	}
	matches.clear();
	for (auto a : matchHolder) {
		//cerr << "name: " << a.second.genomeName << " / len matched: " << a.second.length << " / pos found" << a.second.position << endl;
		matches.push_back(a.second);
	}

	if (matches.empty())
		return false;
	return true;
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	if (fragmentMatchLength < minSL)
		return false;

	results.clear();

	string sequenceFrag;
	unordered_map<string, int> genomeHits;
	int n_seq = 0;
	for (int sPos = 0; query.extract(sPos, fragmentMatchLength, sequenceFrag); sPos += fragmentMatchLength) {
		n_seq++;
		vector<DNAMatch> matches;
		if (findGenomesWithThisDNA(sequenceFrag, fragmentMatchLength, exactMatchOnly, matches)) {
			for (DNAMatch m : matches) {
				if (genomeHits.find(m.genomeName) == genomeHits.end())
					genomeHits[m.genomeName] = 1;
				else
					genomeHits[m.genomeName]++;
			}
		}
	}

	/*cerr << "tried " << n_seq << " sequences" << endl;
	for (auto h : genomeHits) {
		cerr << "hit " << h.first << " with " << h.second << " hits" << endl;
	}*/

	set<pair<double, string>> matchingGenomes;

	for (auto e : genomeHits) {
		double p_match = static_cast<double>(e.second) * 100 / n_seq;
		if (p_match >= matchPercentThreshold) {
			pair<double, string> m(p_match, e.first);
			matchingGenomes.insert(m);
		}
	}

	for (auto it = matchingGenomes.rbegin(); it != matchingGenomes.rend(); it++) {
		GenomeMatch m;
		m.genomeName = it->second;
		m.percentMatch = it->first;
		results.push_back(m);
	}

	return !results.empty();
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
	m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
	delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
	m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
	return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
