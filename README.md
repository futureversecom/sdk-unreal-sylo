# Unreal Sylo SDK

An Unreal Engine plugin that provides a simple API for accessing data associated with a **Sylo Decentralized Identifier (DID)**.

This SDK allows your game or experience to resolve and retrieve data stored under a Sylo DID using either Blueprints or C++.

---

## 🔍 What is Sylo?

**Sylo** is a protocol for accessing off-chain decentralized data associated with a **DID (Decentralized ID)**. A Sylo DID looks like:

```
did:sylo-data:0xfFFFffFF0000000000000000000000000000052f/ed38c341-a26a-4426-aed9-4f8f362b70bf
```

This structure breaks down as follows:

- `did:` → Declares this is a decentralized identifier.
- `sylo-data:` → Specifies the method used, in this case `sylo-data`.
- `0xfFFFffFF0000000000000000000000000000052f` → The owner address of the data.
- `ed38c341-a26a-4426-aed9-4f8f362b70bf` → A unique ID for the data being accessed.

---

## 🚀 Features

- ✅ Load Sylo DID data using **Blueprints** or **C++**
- ✅ Uses a **subsystem** pattern for easy integration into game instances
- ✅ Supports **token-based authentication**
- ✅ Built-in support for **asynchronous Futures** and **delegates**

---

## 🔧 Getting Started

### 1. Add the SDK to your project

Add the `UnrealSyloPlugin` to your project's plugins folder and enable it in the **Plugin Manager**.

### 2. Initialize the Sylo Subsystem

The `USyloSubsystem` is automatically created by Unreal if your GameInstance inherits from `UGameInstance`. You can access it like this:

```cpp
USyloSubsystem* SyloSubsystem = GetGameInstance()->GetSubsystem<USyloSubsystem>();
```

---

## 🔐 Authentication Setup

Before loading a DID, you must configure an **Access Source** which provides bearer tokens.

```cpp
TSharedPtr<ISyloAccessSource> MyAccessSource = MakeShared<FMyCustomAccessSource>();
SyloSubsystem->SetSyloAccessSource(TEXT("MyResolverID"), MyAccessSource);
```

Your custom class must implement the `ISyloAccessSource` interface, providing methods for retrieving and refreshing access tokens.

---

## 📦 Loading Sylo Data

### Blueprint Example

![image](https://github.com/user-attachments/assets/eab3c5ca-38d9-42e0-bf64-9f05b9e31964)

### C++ Example

```cpp
SyloSubsystem->LoadSyloDIDFuture(TEXT("did:sylo-data:..."))
	.Next([](FSyloLoadResult Result)
{
	if (Result.bSuccess && Result.Data.IsValid())
	{
		// Process data
	}
});
```

---

## 📘 FSyloLoadResult

Returned by all data load methods:

| Property | Type                     | Description                            |
|----------|--------------------------|----------------------------------------|
| bSuccess | `bool`                   | Whether the data load was successful   |
| Data     | `TSharedPtr<TArray<uint8>>` | Binary data, null if load failed        |

---

## 🧩 Dependencies

- Unreal Engine 5.3+ (Subsystems, Async/Future support)

---

## 📝 License

© 2025 Futureverse Corporation Limited. All rights reserved.
