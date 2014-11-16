#include <iostream>
#include <random>
#include <time.h>

namespace PokerGame {
	enum Suit { CLUBS, DIAMONDS, HEARTS, SPADES };
	enum Rank { TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };
	enum HandType { HIGHCARD, STRAIGHT, FLUSH, FULLHOUSE, FOUROFAKIND, STRAIGHTFLUSH };

	const std::string suitStrings[] = { "CLUBS", "DIAMONDS", "HEARTS", "SPADES" };
	const std::string rankStrings[] = { "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING", "ACE"};
	const std::string handTypeStrings[] = { "HIGHCARD", "STRAIGHT", "FLUSH", "FULLHOUSE", "FOUROFAKIND", "STRAIGHTFLUSH" };

	class Card {
	public:
		Suit suit;
		Rank rank;

		std::string getCardString() {
			return rankStrings[rank] + " of " + suitStrings[suit];
		}
	};

	class Deck {
	private:
		Card cards[52];
		int cardPtr;

	public:
		Deck() {
			//Shuffle the cards
			srand((unsigned long) time(NULL));
			int base[52];
			for (int i = 0; i < 52; i++)
				base[i] = i;

			for (int i = 0; i < 52; i++)
			{
				int r = rand() % (52 - i);
				cards[i].suit = (Suit) (base[r] % 4);
				cards[i].rank = (Rank) (base[r] >> 2); //(r / 4)
				base[r] = base[51 - i];
			}

			cardPtr = 0;
		}

		Card popCard() {
			return cards[cardPtr++];
		}
	};

	class Hand {
	friend class HandAnalysis;
	public:
		Card cards[5];
		int cardPtr;

		HandType handType;
		Rank handRank;

	public:
		Hand() {
			cardPtr = 0;
		}

		void pushCard(Card card) {
			cards[cardPtr++] = card;
		}

		void printHand() {
			for (int i = 0; i < 5; i++)
				std::cout << cards[i].getCardString() << std::endl;
			std::cout << "HandType: " << handTypeStrings[(int)handType] << ", " << rankStrings[(int)handRank] << std::endl;
		}

		bool operator>(Hand const& hand) {
			return (this->handType > hand.handType) || (this->handType == hand.handType && this->handRank > hand.handRank);
		}

		bool operator==(Hand const& hand) {
			return (this->handType == hand.handType) && (this->handRank == hand.handRank);
		}

		bool operator>=(Hand const& hand) {
			return *this > hand || *this == hand;
		}
	};

	class Dealer { //Singleton Class
	private:
		Deck deck;
		Dealer(){}
		Dealer(Dealer const&){}
		Dealer& operator=(Dealer const&){}
		static Dealer* _dealerInstance;

	public:
		static Dealer* Instance() {
			if (!_dealerInstance)
      			_dealerInstance = new Dealer;
   			return _dealerInstance;
		}

		Card dealCard() {
			return deck.popCard();
		}

		Hand dealHand() {
			Hand hand;
			for (int i = 0; i < 5; i++)
				hand.pushCard(deck.popCard());
			return hand;
		}
	};

	class HandAnalysis {
	private:
		HandAnalysis(){}
		HandAnalysis(HandAnalysis const&){}
		HandAnalysis& operator=(HandAnalysis const&){}
		static HandAnalysis* _haInstance;
	public:
		static HandAnalysis* Instance() {
			if (!_haInstance)
      			_haInstance = new HandAnalysis;
   			return _haInstance;
		}

		void handAnalysis(Hand &hand) {
			int rankCount[13];
			int suitCount[4];

			// preprocessing
			std::fill_n(rankCount, 13, 0);
			std::fill_n(suitCount, 4, 0);

			for (int i = 0; i < 5; i++) {
				rankCount[hand.cards[i].rank]++;
				suitCount[hand.cards[i].suit]++;
			}

			// start hand analysis
			hand.handType = HIGHCARD; // assume high card
			int straightCount = 0;
			int threeOfAKind = -1;
			int twoOfAKind = -1;
			for (int i = 0; i < 13; i++) {
				switch(rankCount[i]) {
					case 4:
						hand.handType = FOUROFAKIND;
						hand.handRank = (Rank)i;
						return;
					case 3:
						threeOfAKind = i;
						break;
					case 2:
						twoOfAKind = i;
						break;
					case 1:
						hand.handRank = (Rank)i;
						if (++straightCount == 5)
							hand.handType = STRAIGHT;
						break;
					default:
						straightCount = 0;
						break;
				}
			}

			if (threeOfAKind >= 0 && twoOfAKind >= 0) {
				hand.handType = FULLHOUSE;
				hand.handRank = (Rank)threeOfAKind;
				return;
			}

			for (int i = 0; i < 4; i++ )
				if (suitCount[i] == 5) {
					if (hand.handType == STRAIGHT)
						hand.handType = STRAIGHTFLUSH;
					else
						hand.handType = FLUSH;
					return;
				}
		}

		void printHandCompare(Hand hand1, Hand hand2) {
			if (hand1 > hand2)
				std::cout << "Hand1 wins!" << std::endl;
			else if (hand2 > hand1)
				std::cout << "Hand2 wins!" << std::endl;
			else
				std::cout << "Draw!" << std::endl;
		}
	};

	Dealer* Dealer::_dealerInstance = NULL;
	HandAnalysis* HandAnalysis::_haInstance = NULL;
}

using namespace PokerGame;
using namespace std;

int main(int argc, char *argv[]) {

	Hand hand1 = Dealer::Instance()->dealHand();
	Hand hand2 = Dealer::Instance()->dealHand();

	cout << "Hand1: " << endl;
	HandAnalysis::Instance()->handAnalysis(hand1);
	hand1.printHand();

	cout << endl << "Hand2: " << endl;
	HandAnalysis::Instance()->handAnalysis(hand2);
	hand2.printHand();

	cout << endl;
	HandAnalysis::Instance()->printHandCompare(hand1, hand2);

	/* this section is used to specify cards
	cout << endl << "Test hand1: " << endl;
	Hand hand3;
	hand3.cards[0].rank = (Rank)0;
	hand3.cards[1].rank = (Rank)0;
	hand3.cards[2].rank = (Rank)0;
	hand3.cards[3].rank = (Rank)0;
	hand3.cards[4].rank = (Rank)3;

	hand3.cards[0].suit = (Suit)0;
	hand3.cards[1].suit = (Suit)1;
	hand3.cards[2].suit = (Suit)2;
	hand3.cards[3].suit = (Suit)3;
	hand3.cards[4].suit = (Suit)1;
	HandAnalysis::Instance()->handAnalysis(hand3);
	hand3.printHand();

	cout << endl << "Test hand2: " << endl;
	Hand hand4;
	hand4.cards[0].rank = (Rank)0;
	hand4.cards[1].rank = (Rank)0;
	hand4.cards[2].rank = (Rank)0;
	hand4.cards[3].rank = (Rank)3;
	hand4.cards[4].rank = (Rank)3;

	hand4.cards[0].suit = (Suit)0;
	hand4.cards[1].suit = (Suit)1;
	hand4.cards[2].suit = (Suit)2;
	hand4.cards[3].suit = (Suit)3;
	hand4.cards[4].suit = (Suit)1;
	HandAnalysis::Instance()->handAnalysis(hand4);
	hand4.printHand();

	cout << endl;
	HandAnalysis::Instance()->printHandCompare(hand3, hand4);
	*/

	return 0;
}