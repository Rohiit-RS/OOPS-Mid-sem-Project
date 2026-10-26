// Game.cpp
#include "Game.h"

int Game::gamesPlayed = 0;

Game::Game() {
    deck = new Deck();
    current = 0;
    dir = 1;
    dealer = 0;
    activeColor = 'R';
}

Game::Game(const Game& o) {
    players = o.players;
    deck = new Deck(*o.deck);
    current = o.current;
    dir = o.dir;
    dealer = o.dealer;
    activeColor = o.activeColor;
    events = o.events;
}

Game::~Game() {
    delete deck;
}

Game& Game::operator=(const Game& o) {
    if (this != &o) {
        Deck* nd = new Deck(*o.deck);
        delete deck;
        deck = nd;
        players = o.players;
        current = o.current;
        dir = o.dir;
        dealer = o.dealer;
        activeColor = o.activeColor;
        events = o.events;
    }
    return *this;
}

// ---------- helpers ----------
int Game::nextIndex(int from, int steps) const {
    int n = static_cast<int>(players.size());
    return (((from + dir * steps) % n) + n) % n;
}

void Game::say(const string& msg) {
    cout << msg;
    events += msg;
}

// Clear mode 1: print blank lines to push old content off the screen.
void Game::clearScreen() const {
    cout << flush;
    for (int i = 0; i < 100; ++i) cout << "\n";
}

void Game::drawCards(int idx, int n) {
    int got = 0;
    for (int i = 0; i < n; ++i) {
        Card c;
        if (deck->draw(c)) {
            players[idx].addCard(c);
            ++got;
        }
    }
    say("  " + players[idx].getName() + " draws " + to_string(got) + " card(s).\n");
}

void Game::showTable() const {
    cout << "\nTop card: " << deck->top() << "   |   Active colour: "
         << Card::colorName(activeColor) << "   |   Direction: "
         << (dir == 1 ? "clockwise" : "counter-clockwise") << "\n";
    cout << "Cards in hand: ";
    for (int i = 0; i < static_cast<int>(players.size()); ++i)
        cout << (i == current ? "[" : "") << players[i].getName() << ":"
             << players[i].handSize() << (i == current ? "]" : "") << "  ";
    cout << "\n";
}

// ---------- setup ----------
void Game::setup() {
    int n = readInt("Number of players (2-10): ", 2, 10);
    for (int i = 0; i < n; ++i) {
        Player p;
        cout << "Name of player " << (i + 1) << " (no spaces): ";
        cin >> p;
        cin.ignore(10000, '\n');
        players.push_back(p);
    }
}

// ---------- choices ----------
int Game::chooseCard(const Player& p) const {
    const Card& top = deck->top();
    cout << "\n" << p.getName() << ", your hand:\n";
    p.showHand(&top, activeColor);
    while (true) {
        int v = readInt("Enter card number to play (0 = draw a card): ", 0, p.handSize());
        if (v == 0) return -1;
        if (p.canPlay(v - 1, top, activeColor)) return v - 1;
        cout << "  That card doesn't match the top card. Try again.\n";
    }
}

Color Game::chooseColor(const Player& p) const {
    int v = readInt(p.getName() + ", choose colour: 1) Red  2) Yellow  3) Green  4) Blue : ", 1, 4);
    return "RYGB"[v - 1];
}

// ---------- card effects ----------
int Game::applyCard(int idx, const Card& c, Color prevActive) {
    deck->discard(c);
    if (!c.isWild()) activeColor = c.getColor();
    int n = static_cast<int>(players.size());
    int skips = 0;

    if (c.isWildDrawFour()) {
        activeColor = chooseColor(players[idx]);
        say("  " + players[idx].getName() + " chooses " + Card::colorName(activeColor) + ".\n");
        int t = nextIndex(idx);
        clearScreen();
        cout << "Pass the device to " << players[t].getName() << " and press Enter when ready...";
        cin.get();
        cout << "\n" << players[idx].getName() << " played Wild Draw Four and chose "
             << Card::colorName(activeColor) << ".\n"
             << "Cards in " << players[idx].getName() << "'s hand: " << players[idx].handSize() << "\n";
        bool challenge = readYesNo(players[t].getName() +
                                   ", do you challenge the Wild Draw Four? (y/n): ");
        if (!challenge) {
            drawCards(t, 4);
            skips = 1;
        } else {
            say("  " + players[t].getName() + " challenges! " + players[idx].getName() +
                " must reveal their hand:\n");
            players[idx].showHand();
            if (players[idx].hasColor(prevActive)) {
                say("  Illegal play! " + players[idx].getName() + " draws 4 instead.\n");
                drawCards(idx, 4);
            } else {
                say("  Legal play - challenge fails. " + players[t].getName() + " draws 6.\n");
                drawCards(t, 6);
                skips = 1;
            }
        }
    } else if (c.isWild()) {
        activeColor = chooseColor(players[idx]);
        say("  " + players[idx].getName() + " chooses " + Card::colorName(activeColor) + ".\n");
    } else if (c.getValue() == 'S') {
        say("  " + players[nextIndex(idx)].getName() + " is skipped!\n");
        skips = 1;
    } else if (c.getValue() == 'R') {
        if (n == 2) {
            say("  Reverse (acts as Skip with 2 players)!\n");
            skips = 1;
        } else {
            dir = -dir;
            say("  Direction reversed!\n");
        }
    } else if (c.getValue() == 'D') {
        int t = nextIndex(idx);
        drawCards(t, 2);
        say("  " + players[t].getName() + " loses their turn.\n");
        skips = 1;
    }
    return skips;
}

// ---------- UNO shout ----------
void Game::unoPhase(int idx) {
    say("      " + players[idx].getName() + ": UNO! (only 1 card left)    \n");
}

// ---------- one turn ----------
int Game::takeTurn() {
    Player& p = players[current];
    Color prev = activeColor;
    Card top = deck->top();

    clearScreen();
    cout << "Pass the device to " << p.getName() << " and press Enter when ready...";
    cin.get();
    cout << "\n=== " << p.getName() << "'s turn ===\n";
    if (!events.empty()) cout << "Since your last turn:\n" << events;
    events.clear();
    showTable();

    p.sortHand();
    int choice = chooseCard(p);

    Card played;
    bool didPlay = false;
    if (choice >= 0) {
        played = p.playCard(choice);
        didPlay = true;
    } else {
        Card drawn;
        if (deck->draw(drawn)) {
            p.addCard(drawn);
            int idx = p.handSize() - 1;
            cout << "You drew: " << drawn << "\n";
            if (p.canPlay(idx, top, prev) &&
                readYesNo("Play it now? (y/n): ")) {
                played = p.playCard(idx);
                didPlay = true;
            }
            if (!didPlay) say(p.getName() + " draws a card and passes.\n");
        } else {
            say(p.getName() + " cannot draw (no cards left) and passes.\n");
        }
    }

    int skips = 0;
    if (didPlay) {
        say(p.getName() + " plays " + played.toString() + ".\n");
        skips = applyCard(current, played, prev);
        if (p.handSize() == 0) return current;
        if (p.handSize() == 1) unoPhase(current);
    }
    cout << "\nPress Enter to hide the screen...";
    cin.get();
    current = nextIndex(current, 1 + skips);
    return -1;
}

// ---------- start card ----------
void Game::handleStartCard(const Card& start) {
    int leftPlayer = nextIndex(dealer);
    activeColor = start.getColor();
    say("Starting card: " + start.toString() + "\n");
    if (start.isWild()) {
        activeColor = chooseColor(players[leftPlayer]);
        say("  " + players[leftPlayer].getName() + " declares " + Card::colorName(activeColor) + ".\n");
        current = leftPlayer;
    } else if (start.getValue() == 'D') {
        say("  " + players[leftPlayer].getName() + " draws 2 and forfeits the first turn.\n");
        drawCards(leftPlayer, 2);
        current = nextIndex(leftPlayer);
    } else if (start.getValue() == 'R') {
        dir = -1;
        current = dealer;
        say("  Direction is counter-clockwise; dealer " + players[dealer].getName() + " starts.\n");
    } else if (start.getValue() == 'S') {
        say("  " + players[leftPlayer].getName() + " is skipped.\n");
        current = nextIndex(dealer, 2);
    } else {
        current = leftPlayer;
    }
}

// ---------- round / game ----------
void Game::endGame(int winner) {
    ++gamesPlayed;
    cout << "\n\n  " << players[winner].getName()
         << " played their last card and WINS THE GAME!\n\n"
         << "Cards left in the other hands:\n";
    for (int i = 0; i < static_cast<int>(players.size()); ++i)
        if (i != winner) cout << "  " << players[i] << "\n";
}

void Game::playGame() {
    deck->reset();
    int n = static_cast<int>(players.size());
    for (int i = 0; i < n; ++i) players[i].clearHand();
    dealer = gamesPlayed % n;
    dir = 1;
    events.clear();

    for (int r = 0; r < 7; ++r)
        for (int i = 0; i < n; ++i) {
            Card c;
            deck->draw(c);
            players[nextIndex(dealer, i + 1)] += c;
        }

    cout << "\n             NEW GAME (dealer: "
         << players[dealer].getName() << ")             \n";
    handleStartCard(deck->startCard());

    int winner = -1;
    while (winner < 0) winner = takeTurn();
    endGame(winner);
}

void Game::run() {
    setup();
    do {
        playGame();
    } while (readYesNo("\nShuffle the whole deck and start a brand-new match? (y/n): "));
    cout << "Thanks for playing! Games played: " << gamesPlayed << "\n";
}
