#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef vector<ll> vll;
typedef vector<vll> vvll;
#define fori(i,a,n) for(int i = a; i < n; i++)
#define endl '\n'
const int MOD = 1e9+7;
const int INF = INT_MAX;
const long long LLINF = LLONG_MAX;
const double EPS = DBL_EPSILON;
void fastIO() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
}
void setIO() {
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
    #endif
}

ll solve() {
    ll l, r;
    cin >> l >> r;
    ll ans = 0, cif=1, unos=0;

    // cout << l << " " << (1e7+1)<<endl;
    if(l>=(ll)1e18) return 0;

    // antes de l, ingnora
    while(cif <= l) {
        //avance
        cif*=10;
        unos = unos*10+1;
    }
    // cout << ans << " " << cif << " " << unos << endl;


    // juntos [l,r]
    if(cif >= r) {
        for(ll i=unos; i<=r; i+=unos)
            if(i>=l) ans++;
        return ans;
    }

    // [l, cif]
    for(ll i=unos; i<=unos*9; i+=unos)
        if(i>=l) ans++;
    unos = unos*10+1;
    // cout << ans << " " << unos << endl;
    
    // [cifL, cifR]
    while(cif*10 <= r && cif <= (ll)1e17) {
        //avance
        cif*=10;
        unos = unos*10+1;
        ans +=9;
        // cout << ans << " " << cif << " " << unos << endl;
    }

    // [cif, r]
    for(ll i=unos; i<=r; i+=unos) {
        ans++;
    }

    // cout << ans << " " << unos << endl;

    return ans;
}

int main() {
    fastIO();
    ll q; 
    cin >> q;
    while(q--) cout << solve() << endl;
        
    return 0;
}