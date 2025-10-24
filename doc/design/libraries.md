# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libqbitscoin_cli*         | RPC client functionality used by *qbitscoin-cli* executable |
| *libqbitscoin_common*      | Home for common functionality shared by different executables and libraries. Similar to *libqbitscoin_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libqbitscoin_consensus*   | Stable, backwards-compatible consensus functionality used by *libqbitscoin_node* and *libqbitscoin_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libqbitscoinconsensus*    | Shared library build of static *libqbitscoin_consensus* library |
| *libqbitscoin_kernel*      | Consensus engine and support library used for validation by *libqbitscoin_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libqbitscoinqt*           | GUI functionality used by *qbitscoin-qt* and *qbitscoin-gui* executables |
| *libqbitscoin_ipc*         | IPC functionality used by *qbitscoin-node*, *qbitscoin-wallet*, *qbitscoin-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libqbitscoin_node*        | P2P and RPC server functionality used by *qbitscoind* and *qbitscoin-qt* executables. |
| *libqbitscoin_util*        | Home for common functionality shared by different executables and libraries. Similar to *libqbitscoin_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libqbitscoin_wallet*      | Wallet functionality used by *qbitscoind* and *qbitscoin-wallet* executables. |
| *libqbitscoin_wallet_tool* | Lower-level wallet functionality used by *qbitscoin-wallet* executable. |
| *libqbitscoin_zmq*         | [ZeroMQ](../zmq.md) functionality used by *qbitscoind* and *qbitscoin-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libqbitscoin_consensus* and *libqbitscoin_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libqbitscoin_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libqbitscoin_node* code lives in `src/node/` in the `node::` namespace
  - *libqbitscoin_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libqbitscoin_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libqbitscoin_util* code lives in `src/util/` in the `util::` namespace
  - *libqbitscoin_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

qbitscoin-cli[qbitscoin-cli]-->libqbitscoin_cli;

qbitscoind[qbitscoind]-->libqbitscoin_node;
qbitscoind[qbitscoind]-->libqbitscoin_wallet;

qbitscoin-qt[qbitscoin-qt]-->libqbitscoin_node;
qbitscoin-qt[qbitscoin-qt]-->libqbitscoinqt;
qbitscoin-qt[qbitscoin-qt]-->libqbitscoin_wallet;

qbitscoin-wallet[qbitscoin-wallet]-->libqbitscoin_wallet;
qbitscoin-wallet[qbitscoin-wallet]-->libqbitscoin_wallet_tool;

libqbitscoin_cli-->libqbitscoin_util;
libqbitscoin_cli-->libqbitscoin_common;

libqbitscoin_common-->libqbitscoin_consensus;
libqbitscoin_common-->libqbitscoin_util;

libqbitscoin_kernel-->libqbitscoin_consensus;
libqbitscoin_kernel-->libqbitscoin_util;

libqbitscoin_node-->libqbitscoin_consensus;
libqbitscoin_node-->libqbitscoin_kernel;
libqbitscoin_node-->libqbitscoin_common;
libqbitscoin_node-->libqbitscoin_util;

libqbitscoinqt-->libqbitscoin_common;
libqbitscoinqt-->libqbitscoin_util;

libqbitscoin_wallet-->libqbitscoin_common;
libqbitscoin_wallet-->libqbitscoin_util;

libqbitscoin_wallet_tool-->libqbitscoin_wallet;
libqbitscoin_wallet_tool-->libqbitscoin_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class qbitscoin-qt,qbitscoind,qbitscoin-cli,qbitscoin-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libqbitscoin_wallet* and *libqbitscoin_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libqbitscoin_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libqbitscoin_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libqbitscoin_common* should serve a similar function as *libqbitscoin_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libqbitscoin_util* and *libqbitscoin_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for qbitscoin-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libqbitscoin_kernel* is not supposed to depend on *libqbitscoin_common*, only *libqbitscoin_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libqbitscoin_kernel* should only depend on *libqbitscoin_util* and *libqbitscoin_consensus*.

- The only thing that should depend on *libqbitscoin_kernel* internally should be *libqbitscoin_node*. GUI and wallet libraries *libqbitscoinqt* and *libqbitscoin_wallet* in particular should not depend on *libqbitscoin_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libqbitscoin_consensus*, *libqbitscoin_common*, and *libqbitscoin_util*, instead of *libqbitscoin_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libqbitscoinqt*, *libqbitscoin_node*, *libqbitscoin_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libqbitscoin_node* to *libqbitscoin_kernel* as part of [The libqbitscoinkernel Project #24303](https://github.com/qbitscoin/qbitscoin/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/qbitscoin/qbitscoin/issues/15732)
