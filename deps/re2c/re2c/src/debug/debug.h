#ifndef _RE2C_DEBUG_DEBUG_
#define _RE2C_DEBUG_DEBUG_


#ifndef RE2C_DEBUG

namespace re2c {

struct opt_t;

struct closure_stats_t {};
struct dump_dfa_t { dump_dfa_t(const opt_t *) {} };

#define DASSERT(x)
#define DDUMP_NFA(opts, nfa)
#define DDUMP_DFA_RAW(ctx, isnew)
#define DDUMP_DFA_DET(opts, dfa)
#define DDUMP_DFA_TAGOPT(opts, dfa)
#define DDUMP_DFA_MIN(opts, dfa)
#define DDUMP_ADFA(opts, adfa)
#define DDUMP_CLSTATS(ctx)
#define DDUMP_CFG(opts, cfg, live)
#define DDUMP_INTERF(opts, cfg, itf)
#define DINCCOUNT_CLSCANS(ctx)
#define DINCCOUNT_CLPREC(ctx)
#define DINCCOUNT_CLLENGTH(ctx, len)
#define DRESET_CLSTATS(ctx)

} // namespace re2c

#else // RE2C_DEBUG

#include <assert.h>
#include <cstdint>

namespace re2c {

struct DFA;
struct cfg_t;
struct determ_context_t;
struct dfa_t;
struct nfa_t;
struct opt_t;
struct tcmd_t;

struct closure_stats_t
{
    uint32_t nscans; // number of configuration scans
    uint32_t nprec;  // number of POSIX comparisons of configurations
    size_t length;   // total length of compared histories
};

struct dump_dfa_t
{
    const bool debug;
    uint32_t uniqidx;

    explicit dump_dfa_t(const opt_t *);
    ~dump_dfa_t();
    void state(const determ_context_t &, bool);
};

#define DASSERT(x) assert(x)
#define DDUMP_NFA(opts, nfa)         if (opts->dump_nfa) dump_nfa(nfa)
#define DDUMP_DFA_RAW(ctx, isnew)    ctx.dc_dump.state(ctx, is_new);
#define DDUMP_DFA_DET(opts, dfa)     if (opts->dump_dfa_det) dump_dfa(dfa)
#define DDUMP_DFA_TAGOPT(opts, dfa)  if (opts->dump_dfa_tagopt) dump_dfa(dfa)
#define DDUMP_DFA_MIN(opts, dfa)     if (opts->dump_dfa_min) dump_dfa(dfa)
#define DDUMP_ADFA(opts, adfa)       if (opts->dump_adfa) dump_adfa(adfa)
#define DDUMP_CLSTATS(ctx)           dump_clstats(ctx)
#define DDUMP_CFG(opts, cfg, live)   if (opts->dump_cfg) dump_cfg(cfg, live)
#define DDUMP_INTERF(opts, cfg, itf) if (opts->dump_interf) dump_interf(cfg, itf)
#define DINCCOUNT_CLSCANS(ctx)       ++ctx.dc_clstats.nscans
#define DINCCOUNT_CLPREC(ctx)        ++ctx.dc_clstats.nprec
#define DINCCOUNT_CLLENGTH(ctx, len) ctx.dc_clstats.length += len
#define DRESET_CLSTATS(ctx)          reset_clstats(ctx)

void dump_nfa(const nfa_t &);
void dump_dfa(const dfa_t &);
void dump_adfa(const DFA &);
void dump_cfg(const cfg_t &, const bool *);
void dump_interf(const cfg_t &, const bool *);
void dump_clstats(const determ_context_t &);
void dump_tcmd(const tcmd_t *);
void reset_clstats(determ_context_t &);

} // namespace re2c

#endif // RE2C_DEBUG

#endif // _RE2C_DEBUG_DEBUG_
