# IntentKernel Migration Architecture v0.4

IntentKernel migration provides a lightweight mechanism to capture installed apps, settings, and capability grants in a portable manifest.

## Components

- **Migration manifest** (`include/migrate.h`, `lib/migrate.c`): key-value text format for apps, settings, and capability grants.
- **Capability manifest** (`include/manifest.h`, `lib/manifest.c`): declarative per-app permissions.
- **Bloatware policy** (`include/policy.h`, `lib/policy.c`): hardcoded categories rejected during restore.
- **Migration daemon** (`user/migrated/`): IPC handler for backup/restore and audit logging.
- **CLI tool** (`tools/intentkernel-migrate`): backup/restore/validate/list operations.
- **IK-MOS integration** (`ikmos/subsystems/migrate_system.py`): `migrate` shell command.

## Manifest Format

```
user_data=/home/user
app=demo_app
category=system
setting=region=us
grant=demo_app:CAP_FILE_READ
```

## Commands

```
intentkernel-migrate backup [path]
intentkernel-migrate restore <archive>
intentkernel-migrate validate <manifest>
intentkernel-migrate list-apps
intentkernel-migrate list-caps <app>
intentkernel-migrate status
```
