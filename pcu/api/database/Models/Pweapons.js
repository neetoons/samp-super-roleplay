module.exports = (sequelize, DataTypes) => {
    return sequelize.define("pweapons",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            weaponid: {
                type: DataTypes.INTEGER,
                allowNull: false
            },
            ammo: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 1
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};